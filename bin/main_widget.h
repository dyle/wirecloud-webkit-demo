/*
 * main_widget.h
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

 
#ifndef __MAIN_WIDGET_H_
#define __MAIN_WIDGET_H_


// ------------------------------------------------------------
// incs

// Qt
#include <QtCore/QStringList>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QTextEdit>


// QtWebKit suffers from some poor coding quality :(
#if __GNUC__ == 4 && __GNUC_MINOR__ >= 6
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-pedantic"
#   include <QtWebKit/QWebView>
#   pragma GCC diagnostic pop
#else
#   include <QtWebKit/QWebView>
#endif

// our crisma stuff
#include "about_dialog.h"
#include "event_sink.h"


// ------------------------------------------------------------
// decl


namespace crisma {
    

/**
 * This class represenst the main widget of 
 * the Wirecloud Webkit native demo client
 */
class main_widget : public QFrame {

    
    Q_OBJECT
    
    
public:


    /**
     * ctor
     * 
     * @param   cParent     parent object
     */
    main_widget(QWidget * cParent);
    
    
    /**
     * dtor
     */
    virtual ~main_widget();
    
    
    /**
     * get the list of current good known URLs
     */
    QStringList urls();
    

    /**
     * set the list of good known URLs
     */
    void set_urls(QStringList & cURLs);
    

public slots:


    /**
     * add a new event
     */
    void add_event(QString sEvent);
    
    
    /**
     * load the John Doe special app
     */
    void load_john_doe();
    
    
private slots:


    /**
     * show about stuff
     */
    void about();
    
    
    /**
     * clear the log window
     */
    void clear_log();
    
    
    /**
     * go to url
     */
    void go();
    
    
    /**
     * site loaded
     */
    void loaded(bool bOk);
    

signals:
    
    
    /**
     * the window shoudl close
     */
    void close();

    
private slots:
    
    
    /**
     * java script engine't top window object has been cleared
     */
    void javaScriptWindowObjectCleared();    
    
    
private:
    
    
    /**
     * process john doe log on form
     */
    void process_john_doe_login();


    /**
     * setup the user items
     */
    void setup_ui();


    /**
     * widgets we handle
     */
    struct {
        
        QComboBox * cCmbURL;                /**< insert URL here */
        QWebView * cWbWebkit;               /**< the webkit page */
        QTextEdit * cEdtEventLog;           /**< event log */
        
    } m_cWidgets;
    
    
    /**
     * the about dialog
     */
    about_dialog * m_cDlgAbout;
    
    
    /**
     * this object captures the JavaScript stuff
     */
    event_sink * m_cEventSink;
    
    
    /**
     * flag for loading the JohnDoe account
     */
    bool m_bLoadJohnDoe;
};


}

#endif
