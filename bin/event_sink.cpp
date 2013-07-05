/*
 * event_sink.cpp
 * 
 * the JavaScript event sink
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

#include <QtGui/QMessageBox>

#include "event_sink.h"

using namespace crisma;


// ------------------------------------------------------------
// code


/**
 * ctor
 * 
 * @param   cParent     parent object
 */
event_sink::event_sink(QObject * cParent) : QObject(cParent), m_sName("crisma::event_sink") {
}


/**
 * get name property
 */
QString event_sink::name() const {
    return m_sName;
}


/**
 * we got an event from an WireCloud widget
 * 
 * @param   sJSON       JSON object describing the event
 */
void event_sink::slot(QString sJSON) {
    emit event(QString("event:\n%1").arg(sJSON));
    
}


/**
 * set name property
 */
void event_sink::set_name(QString sName) {
    m_sName = sName;
}


