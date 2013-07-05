/*
 * main_window.h
 * 
 * the main window
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

 
#ifndef __MAIN_WINDOW_H_
#define __MAIN_WINDOW_H_


// ------------------------------------------------------------
// incs

// Qt
#include <QtGui/QCloseEvent>
#include <QtGui/QMainWindow>


// ------------------------------------------------------------
// decl


namespace crisma {
    

// fwd. decl.
class main_widget;

    
/**
 * This class represenst the main window of wirecloud webkit demo
 */
class main_window : public QMainWindow {
    

    Q_OBJECT
    
    
public:


    /**
     * ctor
     */
    main_window();
    
    
    /**
     * dtor
     */
    virtual ~main_window() {};
    
    
protected:


    /**
     * handle close event
     *
     * @param   cEvent      the event passed
     */
    virtual void closeEvent(QCloseEvent* cEvent);
    
    
protected slots:
    
    
    /**
     * quit app
     */
    virtual void quitApp();
    
    
private:
    
    
    /**
     * centers the window on the desktop with default width and height
     */
    void center_window();
    
    
    /**
     * setup the user items
     */
    void setup_ui();
    
    
    /**
     * the main widget 
     */
    main_widget * m_cMainWidget;    
    
};


}

#endif
