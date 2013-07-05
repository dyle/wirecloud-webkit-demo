/*
 * about_dialog.h
 * 
 * declares the about dialog
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

 
#ifndef __ABOUT_DIALOG_H_
#define __ABOUT_DIALOG_H_


// ------------------------------------------------------------
// incs

// Qt
#include <QDialog>


// ------------------------------------------------------------
// decl


namespace crisma {
    
/**
 * This dialog show what this is all about
 */
class about_dialog : public QDialog {
    

    Q_OBJECT
    
    
public:


    /**
     * ctor
     * 
     * @param   cParent     parent widget
     */
    about_dialog(QWidget * cParent = nullptr);
    
    
    /**
     * dtor
     */
    virtual ~about_dialog() {};


private:    
    
    
    /**
     * setup the widget
     */
    void setup_widget();
};


}



#endif
