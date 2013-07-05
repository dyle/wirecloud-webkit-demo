/*
 * environment.h
 * 
 * declares some operating system environment specific items
 *
 * Autor: Oliver Maurhart, <oliver.maurhart@ait.ac.at>
 *
 * Copyright (C) 2012, 2013 AIT Austrian Institute of Technology
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

 
#ifndef __ENVIRONMENT_H_
#define __ENVIRONMENT_H_


// ------------------------------------------------------------
// incs

#include <set>
#include <string>
#include <boost/filesystem.hpp>

// ------------------------------------------------------------
// decls


namespace crisma {
    

/**
 * describe network interfaces of the current system
 * 
 * hence, this model here assumes that interfaces have
 * only 1 (!) IPv4 and only 1 (!) IPv6 address.
 * 
 * (Yes, this is a bit naive ... I know ... but works for now)
 */    
struct nic {
    
    std::string sName;          /**< name of the network interface */
    std::string sIPv4;          /**< IPv4 address */
    std::string sIPv6;          /**< IPv6 address */
    
    
    /**
     * check if this is a valid interface at all
     * 
     * @return  true, if this represents a valid interface
     */
    inline bool valid() const { return !sName.empty(); };
};    
    

/**
 * this class holds some operating system environment specifc items
 */
class environment {


public:


    /**
     * retrieves the path to configuration files
     * 
     * On UNIX system this should be /etc, else the
     * process_image_path()
     *
     * @return  a path to the configuration files
     */
    static boost::filesystem::path config_path();


    /**
     * retrieves the path to the current working directory
     *
     * @return  a path to the current working directory
     */
    static boost::filesystem::path current_path();


    /**
     * retrieves the path to the data directory
     *
     * @param   sApplication        application name
     * @return  a path to the data directory
     */
    static boost::filesystem::path data_path(std::string const & sApplication);


    /**
     * get the name of the nic passing on to the default gateway
     * 
     * @return  the nic pointing to the default gateway
     */
    static crisma::nic default_gateway();


    /**
     * try to locate a given file
     * 
     * this will search a given file in this order:
     * 
     *  1. current path
     *  2. home path
     *  3. data path
     *
     * @param   sFile               a file name
     * @param   sApplication        application name to search in data path
     * @return  path found file (or empty of not found)
     */
    static boost::filesystem::path find_path(std::string const & sFile, std::string const & sApplication = "");


    /**
     * retrieves the path to the user's home
     *
     * @return  a path to home
     */
    static boost::filesystem::path const & home_path();
    
    
    /**
     * make a host name lookup and return list of found addresses
     * 
     * @param   sHost       the host to lookup
     * @param   bIPv4       return IPv4 addresses
     * @param   bIPv6       return IPv6 addresses
     * @return  list of found addresses (IPv4 and IPv6)
     */
    static std::set<std::string> host_lookup(std::string sHost, bool bIPv4 = true, bool bIPv6 = true);
    

    /**
     * get the list of network interfaces of the system
     * 
     * @return  all available network interface of the sytem
     */
    static std::map<std::string, crisma::nic> nics();


    /**
     * retrieves the prefix path
     *
     * @return  the prefix path
     */
    static boost::filesystem::path const & prefix_path();


    /**
     * retrieves the process id (pid) of the current process
     *
     * @return  the process id of the current process
     */
    static unsigned int process_id();
    
    
    /**
     * retrieves the path to the current process image
     *
     * @return  the path to the current process image
     */
    static boost::filesystem::path const & process_image_path();


    /**
     * returns the OS program to open files of any MIME type
     * 
     * this returns the OS program if you e.g. want to open
     * pdf files:
     * 
     * Example: 
     * 
     *  open_mime("application/pdf") gives 
     *      "xdg-open" on Unix
     *      "start" on Windows
     * 
     * @return  the name of the process to open any MIME type
     */
    static std::string open_mime();
    
    
    /**
     * retrieves the path to the system's temp path
     *
     * @return  the path to the system's temp path
     */
    static boost::filesystem::path temp_path();


private:


    /**
     * dtor - no instances or derived classes allowed
     */
    ~environment() {};

};


    
}

#endif
