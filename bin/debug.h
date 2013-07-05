/*
 * debug.h
 * 
 * debug subsystem
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

 

#ifndef __DEBUG_H_
#define __DEBUG_H_


// ------------------------------------------------------------
// incl

#include <string>
#include <sstream>
#include <vector>


// ------------------------------------------------------------
// decls


namespace crisma {
    
/**
 * This is the debug facility.
 * 
 * One creates an instance on the fly and on instance
 * termination the buffered data is flushed to std::cerr.
 * 
 * Example:
 * 
 * 
 *      qkd::utility::debug() << "This is line " << __LINE__ << " and a double: " << 14.3; 
 * 
 */
class debug : public std::stringstream {
    
    
public:    
    
    
    /**
     * dtor
     */
    virtual ~debug() { flush(); }
    
    
    /**
     * the debug flag
     * 
     * @return  the reference to the global wide debug flag
     */
    static bool & enabled();
    
    
    /**
     * write to log
     */
    void flush();
 

    /**
     * the history of the debug messages so far
     * 
     * @return  the debug messages so far
     */
    static std::list<std::string> const & history();
    
    
    /**
     * size of the history buffer
     * 
     * @return  the history buffer size
     */
    static uint64_t & history_size();
    
    
    /**
     * sets the callback function for new logs
     * 
     * @param   fLog        the callback function
     */
    static void set_callback(void (* fLog)(std::string const & ));
    
};


/**
 * dumps a C style array as a string as "{i_0, i_1, ... i_n}"
 * 
 * @param   a       the array to dump
 * @param   n       the size of the array
 * @return  a string listening the elements of the vector
 */
template<class T> std::string debug_array(T * const & a, uint64_t n) {
    bool bFirst = true; 
    std::stringstream ss; 
    ss << "{"; 
    for (uint64_t i = 0; i < n; i++) {
        if (!bFirst) ss << ", "; 
        bFirst = false; 
        ss << a[i]; 
    } 
    ss << "}"; 
    return ss.str(); 
}


/**
 * dumps a std::vector as a string as "{i_0, i_1, ... i_n}"
 * 
 * @param   v       the vector to dump
 * @return  a string listening the elements of the vector
 */
template<class T> std::string debug_vector(std::vector<T> const & v) {
    bool bFirst = true; 
    std::stringstream ss; 
    ss << "{"; 
    for (auto i: v) { 
        if (!bFirst) ss << ", "; 
        bFirst = false; 
        ss << i; 
    } 
    ss << "}"; 
    return ss.str(); 
}


}

#endif


