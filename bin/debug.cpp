/*
 * debug.cpp
 * 
 * Implements the qkd debug class
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

#include "debug.h"

using namespace crisma;


// ------------------------------------------------------------
// vars


/**
 * global debug flag
 */
static bool g_bDebug = false;


/**
 * debug history
 */
static std::list<std::string> g_cHistory;


/**
 * debug history size
 */
static uint64_t g_nHistorySize = 1000;


/**
 * debug callback
 */
void (*g_fLog)(std::string const &) = nullptr;


/**
 * sync output
 */
static std::mutex g_cMutex;


// ------------------------------------------------------------
// code


/**
 * the debug flag
 * 
 * @return  the reference to the global wide debug flag
 */
bool & debug::enabled() {
    return g_bDebug;
}


/**
 * write to log
 */
void debug::flush() {
    
    // don't to any debug if not necessary
    if (!g_bDebug) return;
    
    // dump
    std::string sLine = str();

    {
        // output must be synced
        std::lock_guard<std::mutex> cLock(g_cMutex);
        std::cerr << sLine << std::endl;
    }
    
    // record
    g_cHistory.push_back(sLine + "\n");
    while (g_cHistory.size() >= g_nHistorySize) g_cHistory.pop_front();
    if (g_fLog) (* g_fLog)(sLine);
}


/**
 * the history of the debug messages so far
 * 
 * @return  the debug messages so far
 */
std::list<std::string> const & debug::history() {
    return g_cHistory;
}


/**
 * size of the history buffer
 * 
 * @return  the history buffer size
 */
uint64_t & debug::history_size() {
    return g_nHistorySize;
}



/**
 * sets the callback function for new logs
 * 
 * @param   fLog        the callback function
 */
void debug::set_callback(void (* fLog)(std::string const & )) {
    g_fLog = fLog;
}
