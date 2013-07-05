/*
 * main_window.cpp
 * 
 * implements the main window
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
// incs

#include "baseinc.h"

// Qt
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtGui/QApplication>
#include <QtGui/QCloseEvent>
#include <QtGui/QDesktopWidget>
#include <QtGui/QIcon>
#include <QtGui/QStatusBar>

#include "environment.h"
#include "main_widget.h"
#include "main_window.h"

using namespace crisma;


// ------------------------------------------------------------
// code


/**
 * ctor
 */
main_window::main_window() : QMainWindow(), m_cMainWidget(nullptr) {
    
    setWindowTitle(tr("AIT Wirecloud Webkit native Client Gui V%1").arg(VERSION));
    
    boost::filesystem::path cDataPath = environment::data_path("wirecloud-webkit");
    QString sDataPath = QString::fromStdString(cDataPath.string());
    QPixmap cPixAITLogo = QPixmap(sDataPath + QDir::separator() + "crisma.png");
    if (!cPixAITLogo.isNull()) setWindowIcon(QIcon(cPixAITLogo));
    
    // set central Widget
    m_cMainWidget = new main_widget(this);
    setCentralWidget(m_cMainWidget);

    // load and set any stored settings
    QSettings cSettings("AIT", "wirecloud-webkit");
    if (cSettings.contains("geometry")) restoreGeometry(cSettings.value("geometry").toByteArray());
    else center_window();
    restoreState(cSettings.value("window_state").toByteArray());
    
    // pick list of good URLs
    QStringList cURLs;
    QStringList cKeys = cSettings.allKeys();
    for (int i = 0; i < cKeys.count(); i++) {
        if (cKeys.at(i).left(QString("url_").length()) == QString("url_")) {
            cURLs.append(cSettings.value(cKeys.at(i)).toString());
        }
    }
    
    // special check if empty: first time called, no config saved
    if (cURLs.size() == 0) {
        cURLs.append("http://wirecloud.lab.fi-ware.eu");
        cURLs.append("http://www.crismaproject.eu");
        cURLs.append("http://www.ait.ac.at");
    }
    m_cMainWidget->set_urls(cURLs);
    
    // connectors
    connect(m_cMainWidget, SIGNAL(close()), SLOT(quitApp()));
}


/**
 * centers the window on the desktop with default width and height
 */
void main_window::center_window() {
    
    const int nDefaultWidth = 600;
    const int nDefaultHeight = 600;
    
    QDesktopWidget* cDesktop = qApp->desktop();
    int nX = (cDesktop->width() - nDefaultWidth) / 2;
    int nY = (cDesktop->height() - nDefaultHeight) / 2;
    
    // ready to display
    resize(QSize(nDefaultWidth, nDefaultHeight));
    move(QPoint(nX, nY));
}


/**
 * handle close event
 *
 * @param   cEvent      the event passed
 */
void main_window::closeEvent(QCloseEvent * cEvent) {
    
    QSettings cSettings("AIT", "wirecloud-webkit");

    // remember position
    cSettings.setValue("geometry", saveGeometry());
    cSettings.setValue("window_state", saveState());

    // erase all known URL keys
    QStringList cKeys = cSettings.allKeys();
    for (int i = 0; i < cKeys.count(); i++) {
        if (cKeys.at(i).left(QString("url_").length()) == QString("url_")) cSettings.remove(cKeys.at(i));
    }

    // remember good webkit URLs
    QStringList cURLs = m_cMainWidget->urls();
    for (int i = 0; i < cURLs.count(); i++) {
        cSettings.setValue(QString("url_%1").arg(QString::number(i + 1)), cURLs.at(i));
    }

    QMainWindow::closeEvent(cEvent);
}


/**
 * quit app
 */
void main_window::quitApp() {
    close();
}
