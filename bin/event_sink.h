/*
 * event_sink.h
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

 
#ifndef __EVENT_SINK_H_
#define __EVENT_SINK_H_


// ------------------------------------------------------------
// incs

// Qt
#include <QtCore/QObject>


// ------------------------------------------------------------
// decl


namespace crisma {
    

/**
 * This class gets events from a runnung JavaScript engine
 */
class event_sink : public QObject {

    
    Q_OBJECT
    
    Q_PROPERTY(QString name READ name WRITE set_name)           /**< JavaScript "name" property */
    
    
public:


    /**
     * ctor
     * 
     * @param   cParent     parent object
     */
    event_sink(QObject * cParent);
    
    
    /**
     * dtor
     */
    virtual ~event_sink() {};
    
    
    /**
     * get name property
     */
    QString name() const;
    
    
    /**
     * set name property
     */
    void set_name(QString sName);
    

public slots:
    

    /**
     * we got an event from an WireCloud widget
     * 
     * @param   sJSON       JSON object describing the event
     */
    void slot(QString sJSON);
    
    
signals:
    
    
    /**
     * this is the event issued
     * 
     * @param   sEvent      a string holding the event info
     */
    void event(QString sEvent);
    

private:    
    
    
    QString m_sName;            /**< the name of the object */
    
    
};


}

#endif
