/*
 * about_dialog.cpp
 * 
 * implements the about dialog
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
#include <QDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include "about_dialog.h"
#include "environment.h"

using namespace crisma;


// ------------------------------------------------------------
// vars


/**
 * the about text
 */
QString g_sAboutText = "\
<html>\
<body bgcolor=\"#FF00FF\">\
\
<div align=\"center\">\
<p/>\
<img src=\"image:ait_logo_no_claim.jpg\"/>\
<p/>\
<h1>AIT Crisma <br/>WireCloud-WebKit <br/>Native Client V%1</h1>\
<p/>\
<img src=\"image:crisma_logo.png\"/>\
<p/>\
</div>\
<p/>\
This program examines the possibilities to<br/>\
connect a running WireCloud instance with <br/>\
an already coded C/C++ native client.<br/>\
\
<p>\
Copyright (C) 2013, AIT Austrian Institute of Technology<br/>\
AIT Austrian Institute of Technology GmbH<br/>\
Donau-City-Strasse 1 | 1220 Vienna | Austria<br/>\
<a href=\"http://www.ait.ac.at\">http://www.ait.ac.at</a>\
</p>\
</div>\
\
</body>\
</html>\
";


// ------------------------------------------------------------
// code


/**
 * ctor
 * 
 * @param   cParent     parent widget
 */
about_dialog::about_dialog(QWidget * cParent) : QDialog(cParent) {
    
    setWindowTitle(tr("AIT Crisma WireCloud-WebKit Native Client V%1").arg(VERSION));
    
    setup_widget();
    
    resize(500, 400);
}


/**
 * setup the widget
 */
void about_dialog::setup_widget() {
    
    // define search path
    boost::filesystem::path cDataPath = crisma::environment::data_path("wirecloud-webkit");
    QString sDataPath = QString::fromStdString(cDataPath.string());
    QDir::setSearchPaths("image", QStringList(sDataPath));
    
    // setup widgets
    QVBoxLayout * cLyMain = new QVBoxLayout;
    cLyMain->setContentsMargins(8, 4, 8, 4);
    setLayout(cLyMain);

    QLabel * cLbAboutText = new QLabel;
    cLbAboutText->setStyleSheet("QWidget { background: white; color: black; };");
    cLbAboutText->setText(g_sAboutText.arg(VERSION));
    cLbAboutText->setContentsMargins(12, 12, 12, 12);
    
    QScrollArea * cScrollArea = new QScrollArea;
    cScrollArea->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    cScrollArea->setWidget(cLbAboutText);    
    cScrollArea->viewport()->setStyleSheet("QWidget { background: white; color: black; };");
    cLyMain->addWidget(cScrollArea);
    
    QHBoxLayout * cLyButtons = new QHBoxLayout;
    cLyMain->addLayout(cLyButtons);
    
    cLyButtons->addStretch(1);
    
    QPushButton * cBtDone = new QPushButton(tr("Done"));
    cLyButtons->addWidget(cBtDone);
    
    // connectors
    connect(cBtDone, SIGNAL(clicked(bool)), SLOT(accept()));
}
