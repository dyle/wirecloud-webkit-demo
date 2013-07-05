/*
 * main_widget.cpp
 * 
 * declares the main widget
 * 
 * Autor: Oliver Maurhart, <oliver.maurhart@ait.ac.at>
 *
 * Copyright (C) 2013 AIT Austrian Institute of Technology
 * AIT Austrian Institute of Technology GmbH
 * Donau-City-Strasse 1 | 1220 Vienna | Austria
 * http://www.ait.ac.at
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA  02110-1301, USA.
 */

 
// ------------------------------------------------------------
// include

#include "baseinc.h"

// Qt
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtNetwork/QNetworkRequest>

// QtWebKit suffers from some poor coding quality :(
#if __GNUC__ == 4 && __GNUC_MINOR__ >= 6
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-pedantic"
#   include <QtWebKit/QWebElement>
#   include <QtWebKit/QWebElementCollection>
#   include <QtWebKit/QWebFrame>
#   pragma GCC diagnostic pop
#else
#   include <QtWebKit/QWebElement>
#   include <QtWebKit/QWebElementCollection>
#   include <QtWebKit/QWebFrame>
#endif


#include "main_widget.h"

using namespace crisma;


// ------------------------------------------------------------
// code


/**
 * ctor
 * 
 * @param   cParent     parent object
 */
main_widget::main_widget(QWidget * cParent) : QFrame(cParent), m_bLoadJohnDoe(false) {
    
    // our JavaScript event sink
    m_cEventSink = new event_sink(this);
    connect(m_cEventSink, SIGNAL(event(QString)), SLOT(add_event(QString)));
    
    // pre-init the dialogs (fast show)
    m_cDlgAbout = new about_dialog(this);
    
    // setup the user interface
    setup_ui();
}


/**
 * dtor
 */
main_widget::~main_widget() {
}


/**
 * show about stuff
 */
void main_widget::about() {
    m_cDlgAbout->show();
}


/**
 * add a new event
 */
void main_widget::add_event(QString sEvent) {
    m_cWidgets.cEdtEventLog->append(sEvent);
}


/**
 * clear the log window
 */
void main_widget::clear_log() {
    m_cWidgets.cEdtEventLog->clear();
}

    
/**
 * go to url
 */
void main_widget::go() {
    
    // load a new url into webkit
    m_bLoadJohnDoe = false;
    QString sURL = m_cWidgets.cCmbURL->currentText();
    add_event(QString("Loading URL '%1' ...").arg(sURL));
    m_cWidgets.cWbWebkit->load(QUrl(sURL));
}


/**
 * java script engine't top window object has been cleared
 */
void main_widget::javaScriptWindowObjectCleared() {
    QWebFrame * cWebFrame = m_cWidgets.cWbWebkit->page()->mainFrame();
    cWebFrame->addToJavaScriptWindowObject("crisma_native", m_cEventSink);
}


/**
 * load the John Doe special app
 */
void main_widget::load_john_doe() {
    m_bLoadJohnDoe = true;
    m_cWidgets.cWbWebkit->load(QUrl("http://wirecloud.lab.fi-ware.eu/login?next="));
}


/**
 * site loaded
 */
void main_widget::loaded(bool bOk) {
    
    // push "failed" on error and exit
    if (!bOk) {
        add_event("Failed to load URL");
        return;
    }
    
    // shortcut for john doe
    if (m_bLoadJohnDoe) {
        m_bLoadJohnDoe = false;
        process_john_doe_login();
        return;
    }
    
    // new side loaded --> inject our obejcts
    QWebFrame * cWebFrame = m_cWidgets.cWbWebkit->page()->mainFrame();
    QString sURL = cWebFrame->evaluateJavaScript("window.document.URL").toString();
    QString sTitle = cWebFrame->evaluateJavaScript("window.document.title").toString();
    add_event(QString("Loaded: %1 - \"%2\"").arg(sURL).arg(sTitle));
    
    // check if this is a WireCloud page
    // If so, an "window.opManager" object must be present on the current opage
    QString sTypeOfOpManager = cWebFrame->evaluateJavaScript("typeof window.opManager").toString();
    if (sTypeOfOpManager != "object") {
        add_event("This is not a WireCloud application");
        return;
    }
    
    // we probably do have a wire-cloud app at hand
    add_event("Site is probaly a WireCloud App");
    
    // get the widgets on the workspace
    QVariant cVariant = cWebFrame->evaluateJavaScript("window.opManager.activeWorkspace.wiring.connectablesByWidget");
    if (!cVariant.canConvert<QVariantMap>()) {
        add_event("Nope. Failed to get a grip on the widgets ... ");
        return;
    }
    
    // we probably do have a wire-cloud app at hand
    add_event("Injecting JavaScript code ...");
    
    // nail in our own event slots: 1. create empty slot array
    QString sJSCommand;
    sJSCommand = "window.crisma_sink = []";
    cWebFrame->evaluateJavaScript(sJSCommand);
    
    // nail in our own event slots: 2. connect widget events to our slot
    sJSCommand = "  \
    for (var nWidgetId in opManager.activeWorkspace.wiring.connectablesByWidget) {    \
        var w = opManager.activeWorkspace.wiring.connectablesByWidget[nWidgetId];    \
        for (var sOutputName in w.outputs) {    \
            var o = w.outputs[sOutputName];    \
            var o_name = o._name;    \
            var o_variable_id = o.variable.id;    \
            o.outputs.push({    \
                _friendCode: sOutputName,    \
                _name: o._name,    \
                _type: o._type,    \
                _vid: o.variable.id,    \
                _wid: o.variable.iWidget.id,    \
                _description: o.variable.vardef.description,    \
                propagate: function(value, options) { crisma_native.slot(    \
                    \"{\\n\" +    \
                      \"\\tname:\\t\" + this._name + \",\\n\" +    \
                      \"\\tvariable_id:\\t\" + this._vid + \",\\n\" +    \
                      \"\\tdescription:\\t\" + this._description + \",\\n\" +    \
                      \"\\twidget:\\t\" + this._wid + \",\\n\" +    \
                      \"\\tvalue:\\t\" + value + \"\\n\" +    \
                    \"}\")},    \
                _annotate: function() {}    \
            });    \
        }    \
    }";
    
    // inject main logic
    cWebFrame->evaluateJavaScript(sJSCommand);
    
    // redecorate CSS
    sJSCommand = "                                                                          \
        document.getElementById(\"menu_layer\").style.visibility = \"hidden\";              \
        document.getElementById(\"loading-window\").style.visibility = \"hidden\";          \
        document.getElementById(\"wirecloud_header\").style.visibility = \"hidden\";        \
        a = document.getElementsByClassName(\"container north_container\");                 \
        for (var i = 0; i < a.length; i++) a[i].parentNode.removeChild(a[i]);               \
        a = document.getElementsByClassName(\"container west_container\");                  \
        for (var i = 0; i < a.length; i++) a[i].parentNode.removeChild(a[i]);               \
        a = document.getElementsByClassName(\"container east_container\");                  \
        for (var i = 0; i < a.length; i++) a[i].parentNode.removeChild(a[i]);               \
        a = document.getElementsByClassName(\"container south_container\");                 \
        for (var i = 0; i < a.length; i++) a[i].parentNode.removeChild(a[i]);               \
        a = document.getElementsByClassName(\"container center_container\");                \
        for (var i = 0; i < a.length; i++) a[i].style.top = \"0px\";                        \
        delete window.a;                                                                    \
    ";
    cWebFrame->evaluateJavaScript(sJSCommand);
    
    // trigger relayout of WireCloud WebPage by artifical shrinking and growing widget
    // This is sure a ugly hack to enforce rerendering of the WireCloud DragBox Layout
    // managers .... :P
    m_cWidgets.cWbWebkit->resize(m_cWidgets.cWbWebkit->width(), m_cWidgets.cWbWebkit->height() + 1);
    m_cWidgets.cWbWebkit->resize(m_cWidgets.cWbWebkit->width(), m_cWidgets.cWbWebkit->height() - 1);
    
    // tell we've "enhanced" the sitze
    add_event("WireCloud Mashup assimilated ...");
}


/**
 * process john doe log on form
 */
void main_widget::process_john_doe_login() {
    
    // this should be the login form: fill in our account data and off we go
    QWebFrame * cWebFrame = m_cWidgets.cWbWebkit->page()->mainFrame();
    
    // simulate user behavior ... cheap and easy =)
    cWebFrame->evaluateJavaScript("window.document.forms[0].elements['username'].value = 'john_doe';");
    cWebFrame->evaluateJavaScript("window.document.forms[0].elements['password'].value = '1234';");
    cWebFrame->evaluateJavaScript("window.document.forms[0].submit.click();");
}


/**
 * set the list of good known URLs
 */
void main_widget::set_urls(QStringList & cURLs) {
    m_cWidgets.cCmbURL->clear();
    for (int i = 0; i < cURLs.count(); i++) m_cWidgets.cCmbURL->addItem(cURLs.at(i));
    m_cWidgets.cCmbURL->clearEditText();
}


/**
 * setup the user items
 */
void main_widget::setup_ui() {
    
    setContentsMargins(2, 2, 2, 2);
    
    // main layout
    QVBoxLayout * cLyMain = new QVBoxLayout(this);
    
    // URL input
    QHBoxLayout * cLyURL = new QHBoxLayout;
    cLyMain->addLayout(cLyURL);
    QLabel * cLblURL = new QLabel(tr("&Wirecloud URL:"));
    cLyURL->addWidget(cLblURL);
    m_cWidgets.cCmbURL = new QComboBox;
    m_cWidgets.cCmbURL->setDuplicatesEnabled(false);
    m_cWidgets.cCmbURL->setEditable(true);
    cLyURL->addWidget(m_cWidgets.cCmbURL, 1);
    cLblURL->setBuddy(m_cWidgets.cCmbURL);
    QPushButton * cBtnGo = new QPushButton(tr("Go"));
    cLyURL->addWidget(cBtnGo);
    QPushButton * cBtnJohnDoe = new QPushButton(tr("John Doe"));
    cLyURL->addWidget(cBtnJohnDoe);
    
    // splitter for dispaly and rest
    QSplitter * cSpMain = new QSplitter;
    cSpMain->setOrientation(Qt::Vertical);
    cLyMain->addWidget(cSpMain);
    
    // the webkit page
    QFrame * cFrWebkit = new QFrame(cSpMain);
    
    QVBoxLayout * cLyWebkit = new QVBoxLayout;
    cFrWebkit->setLayout(cLyWebkit);
    
    QLabel * cLblWebkit = new QLabel(tr("Webkit View:"));
    cLyWebkit->addWidget(cLblWebkit);
    m_cWidgets.cWbWebkit = new QWebView;
    cLyWebkit->addWidget(m_cWidgets.cWbWebkit);
    
    // the event log 
    QFrame * cFrEventLog = new QFrame(cSpMain);
    QVBoxLayout * cLyEventLog = new QVBoxLayout;
    cFrEventLog->setLayout(cLyEventLog);
    
    QLabel * cLblEventLog = new QLabel(tr("Webkit Event Log:"));
    cLyEventLog->addWidget(cLblEventLog);
    m_cWidgets.cEdtEventLog = new QTextEdit;
    m_cWidgets.cEdtEventLog->setLineWrapMode(QTextEdit::NoWrap);
    m_cWidgets.cEdtEventLog->setReadOnly(true);
    m_cWidgets.cEdtEventLog->setStyleSheet("QWidget { font-family: \"Monospace,Courier;\"; }");
    cLyEventLog->addWidget(m_cWidgets.cEdtEventLog);
    
    // button area
    QHBoxLayout * cLyButtons = new QHBoxLayout;
    cLyMain->addLayout(cLyButtons);
    cLyButtons->addStretch(1);
    
    QPushButton * cBtnClearLog = new QPushButton(tr("Clear Log"));
    cBtnClearLog->setMinimumWidth(100);
    cLyButtons->addWidget(cBtnClearLog);
    QPushButton * cBtnAbout = new QPushButton(tr("About"));
    cBtnAbout->setMinimumWidth(100);
    cLyButtons->addWidget(cBtnAbout);
    QPushButton * cBtnClose = new QPushButton(tr("Close"));
    cBtnClose->setMinimumWidth(100);
    cLyButtons->addWidget(cBtnClose);
    
    // connectors
    connect(cBtnAbout, SIGNAL(clicked()), SLOT(about()));
    connect(cBtnClearLog, SIGNAL(clicked()), SLOT(clear_log()));
    connect(cBtnClose, SIGNAL(clicked()), SIGNAL(close()));
    connect(cBtnGo, SIGNAL(clicked()), SLOT(go()));
    connect(cBtnJohnDoe, SIGNAL(clicked()), SLOT(load_john_doe()));
    connect(m_cWidgets.cCmbURL, SIGNAL(activated(const QString&)), SLOT(go()));
    connect(m_cWidgets.cCmbURL->lineEdit(), SIGNAL(returnPressed()), SLOT(go()));
    connect(m_cWidgets.cWbWebkit, SIGNAL(loadFinished(bool)), SLOT(loaded(bool)));
    connect(m_cWidgets.cWbWebkit->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), SLOT(javaScriptWindowObjectCleared()));
}


/**
 * get the list of current good known URLs
 */
QStringList main_widget::urls() {
    QStringList cList;
    for (int i = 0; i < m_cWidgets.cCmbURL->count(); i++) cList << m_cWidgets.cCmbURL->itemText(i);
    return cList;
}

