/*
 * environment.cpp
 * 
 * implements functions in environment.h
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

 
// ------------------------------------------------------------
// incs

#include "baseinc.h"
#include "environment.h"

using namespace crisma;


// ------------------------------------------------------------
// code


/**
 * retrieves the path to configuration files
 * 
 * On UNIX system this should be /etc, else the
 * process_image_path()
 *
 * @return  a path to the configuration files
 */
boost::filesystem::path environment::config_path() {
    
    boost::filesystem::path cPath;
    
#ifdef __linux__    

    // ------------------------------------------------------------
    // LINUX implemententation

    cPath = "/etc";
        
#endif    

#ifdef __WIN32__

    // ------------------------------------------------------------
    // WINDOWS implemententation

    // get the full path to the current binary
    // IS the data path

    cPath = get_prefix_path();

#endif    
    
    return cPath;
}


/**
 * retrieves the path to the current working directory
 *
 * @return  a path to the current working directory
 */
boost::filesystem::path environment::current_path() {
    // there is an invalid memory read in 
    // the boost libraries here. At least 
    // boost-1.48-0. This is detected by valgrind 
    // ... can't do something about it.
    // however: no memory leak so far
    return boost::filesystem::current_path();
}


/**
 * retrieves the path to the data directory
 *
 * @param   sApplication        application name
 * @return  a path to the data directory
 */
boost::filesystem::path environment::data_path(std::string const & sApplication) {

    boost::filesystem::path cPath;
    
#ifdef __linux__    

    // ------------------------------------------------------------
    // LINUX implemententation

    // get the prefix and add a "share"
    cPath = prefix_path();
    cPath /= "share";
    if (!sApplication.empty()) cPath /= sApplication;
        
#endif    

#ifdef __WIN32__

    // ------------------------------------------------------------
    // WINDOWS implemententation

    // get the full path to the current binary
    // IS the data path

    cPath = get_prefix_path();

#endif    
    
    return cPath;
}


/**
 * get the name of the nic passing on to the default gateway
 * 
 * @return  the nic pointing to the default gateway
 */
crisma::nic environment::default_gateway() {
    
    crisma::nic cDefaultGateway;
    
#ifdef __linux__    

    // ------------------------------------------------------------
    // LINUX implemententation

    // look at the /proc/net/route "file"
    std::ifstream cRouteFile("/proc/net/route");
    assert(cRouteFile.is_open());
    
    // walk over the routes
    char sLine[1024];
    bool bHeader = true;
    while (!cRouteFile.eof()) {
        
        cRouteFile.getline(sLine, 1024);
        if (bHeader) {
            // ignore the header
            bHeader = false;
            continue;
        }
        
        // default default interface is the one consuming all "0.0.0.0" IP matches
        std::istringstream ss(sLine);
        std::string sInterface;
        std::string sDestination;
        ss >> sInterface;
        ss >> sDestination;
        
        if (sInterface.empty()) continue;
        if (sDestination == "00000000") {
            
            // we have it!
            std::map<std::string, crisma::nic> cNics = nics();
            if (cNics.find(sInterface) == cNics.end()) {
                
                // we have defaut gateway which is *NOT* in the list of available nics! Huh??? WTF! o.O
                return cDefaultGateway;
            }
            
            // pick it, over and out
            cDefaultGateway = cNics[sInterface];
            
            return cDefaultGateway;
        }
    }
    
        
#endif    

#ifdef __WIN32__

    // ------------------------------------------------------------
    // WINDOWS implemententation
    
    // TODO: squeeze the proper network interface out of windows
    
#endif    
    
    return cDefaultGateway;
    
}


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
boost::filesystem::path environment::find_path(std::string const & sFile, std::string const & sApplication) {
    
    boost::filesystem::path cPath(sFile);
    
    // absolute path given?
    if (cPath.is_absolute()) {
        if (boost::filesystem::exists(cPath)) return cPath;
        else return boost::filesystem::path();
    }
    
    // current path ?
    if (boost::filesystem::exists(current_path() / cPath)) {
        return current_path() / cPath;
    }
    
    // home path ?
    if (boost::filesystem::exists(home_path() / cPath)) {
        return home_path() / cPath;
    }
    
    // data path ?
    if (boost::filesystem::exists(data_path(sApplication) / cPath)) {
        return data_path(sApplication) / cPath;
    }
    
    return boost::filesystem::path();
}


/**
 * retrieves the path to the user's home
 *
 * @return  a path to home
 */
boost::filesystem::path const & environment::home_path() {
    static boost::filesystem::path cPath;
    if (cPath.empty()) cPath = boost::filesystem::path(getenv("HOME"));
    return cPath;
}


/**
 * make a host name lookup and return list of found addresses
 * 
 * @param   sHost       the host to lookup
 * @param   bIPv4       return IPv4 addresses
 * @param   bIPv6       return IPv6 addresses
 * @return  list of found addresses (IPv4 and IPv6)
 */
std::set<std::string> environment::host_lookup(std::string sHost, bool bIPv4, bool bIPv6) {

    std::set<std::string> cAddresses;
    
#ifdef __linux__    

    // ------------------------------------------------------------
    // LINUX implemententation

    // call getaddrinfo()
    addrinfo cHint;
    memset(&cHint, 0, sizeof(cHint));
    
    /// setup protocol of interest
    cHint.ai_family = AF_UNSPEC;
    if (bIPv4 && !bIPv6) cHint.ai_family = AF_INET;
    if (!bIPv4 && bIPv6) cHint.ai_family = AF_INET6;
    
    cHint.ai_flags = AI_ALL;
    addrinfo * cAddrList = NULL;
    int nError = getaddrinfo(sHost.c_str(), nullptr, &cHint, &cAddrList);
    
    // fail?
    if (nError) {
        if (cAddrList) freeaddrinfo(cAddrList);
        return cAddresses;
    }
    
    // collect the addresses
    char sNameIPv4[INET_ADDRSTRLEN];
    char sNameIPv6[INET6_ADDRSTRLEN];
    
    std::map<std::string, std::string> cAddressMap;
    
    // walk over the found addresses
    for (addrinfo * cAddr = cAddrList; cAddr != nullptr; cAddr = cAddr->ai_next) {
        
        if (cAddr->ai_family == AF_INET) {
            
            // IPv4
            sNameIPv4[0] = 0;
            if (!getnameinfo(cAddr->ai_addr, cAddr->ai_addrlen, sNameIPv4, INET_ADDRSTRLEN, nullptr, 0, NI_NUMERICHOST)) {
                cAddresses.insert(std::string(sNameIPv4));
            }
        }
        else if (cAddr->ai_family == AF_INET6) {
            
            // IPv6
            sNameIPv6[0] = 0;
            if (!getnameinfo(cAddr->ai_addr, cAddr->ai_addrlen, sNameIPv6, INET6_ADDRSTRLEN, nullptr, 0, NI_NUMERICHOST)) {
                cAddresses.insert(std::string(sNameIPv6));
            }
        }
        else {
            cAddresses.insert("<at least one unknown address familiy>");
        }
    }
    
    // free resources
    if (cAddrList) freeaddrinfo(cAddrList);
    
        
#endif    

#ifdef __WIN32__

    // ------------------------------------------------------------
    // WINDOWS implemententation

    // TODO: do the windows stuff ...

#endif    
    
    
    
    return cAddresses;
}


/**
 * get the list of network interfaces of the system
 * 
 * @return  all available network interface of the sytem
 */
std::map<std::string, crisma::nic> environment::nics() {
    
    std::map<std::string, crisma::nic> cNics;
    
#ifdef __linux__    

    // get a pointer to all nics
    ifaddrs * cIFAddresses = nullptr;
    if (getifaddrs(&cIFAddresses) == -1) {
        
        // TODO: unable to get interface: what to do?
        return cNics;
    }

    // iterate over the found nics          
    for (ifaddrs * cIF = cIFAddresses; cIF != nullptr; cIF = cIF->ifa_next) {
        
        // sanity check
        if (cIF->ifa_addr == nullptr) continue;
            
        // fetch the name
        std::string sNicName(cIF->ifa_name);
        
        // only IPv4 and IPv6 are of interest
        int nFamiliy = cIF->ifa_addr->sa_family;
        if ((nFamiliy == AF_INET) || (nFamiliy == AF_INET6)) {
            
            char sAddress[NI_MAXHOST];
            int nError = getnameinfo(
                cIF->ifa_addr,
                (nFamiliy == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                sAddress, 
                NI_MAXHOST, 
                nullptr, 
                0, 
                NI_NUMERICHOST);
            if (nError != 0) continue;
            
            // place info in the database
            cNics[sNicName].sName = sNicName;
            
            // place the adresses
            if (nFamiliy == AF_INET) cNics[sNicName].sIPv4 = std::string(sAddress);
            if (nFamiliy == AF_INET6) cNics[sNicName].sIPv6 = std::string(sAddress);
        }
    }
    
    // free resources    
    freeifaddrs(cIFAddresses);
   
#endif    

#ifdef __WIN32__

    // ------------------------------------------------------------
    // WINDOWS implemententation

    // TODO: do the windows stuff

#endif    
    
    return cNics;
}


/**
 * retrieves the prefix path
 *
 * @return  the prefix path
 */
boost::filesystem::path const & environment::prefix_path() {
    
    static boost::filesystem::path cPath;
    
    if (cPath.empty()) {
        
        boost::filesystem::path cFullBinaryPath = process_image_path();
        
#ifdef __linux__    

        // ------------------------------------------------------------
        // LINUX implemententation

        // the prefix is the path to the process image, minus the "bin/PROCESS-NAME" part
        cPath = cFullBinaryPath.parent_path().parent_path();

#endif    

#ifdef __WIN32__

        // ------------------------------------------------------------
        // WINDOWS implemententation

        // the prefix on windows IS the path to the process image
        // minus the image
        cPath = cFullBinaryPath.parent_path();

#endif    
    
    }
    
    return cPath;
}


/**
 * retrieves the process id (pid) of the current process
 *
 * @return  the process id of the current process
 */
unsigned int environment::process_id() {
#ifdef __linux__    
    return getpid();
#endif
#ifdef _WIN32
    return GetCurrentProcessId();
#endif    
}


/**
 * retrieves the path to the current process image
 *
 * @return  the path to the current process image
 */
boost::filesystem::path const & environment::process_image_path() {

    static boost::filesystem::path cPath;
    
    if (cPath.empty()) {
        
#ifdef __linux__    

        // ------------------------------------------------------------
        // LINUX implemententation

        // get the full path to the current binary
        // by checking to what /proc/self/exe is pointing to
        
        boost::filesystem::path cSelfExeFile("/proc/self/exe");
        assert(boost::filesystem::is_symlink(cSelfExeFile));
        cPath = boost::filesystem::read_symlink(cSelfExeFile);
        
#endif    

#ifdef __WIN32__

        // ------------------------------------------------------------
        // WINDOWS implemententation

        // get the full path to the current binary
        // by checking the GetModuleName() function

        char sFileName[MAX_PATH];
        GetModuleFileName(GetModuleHandle(nullptr), sFileName, MAX_PATH);
        cPath = boost::filesystem::path(sFileName);

#endif    
    
    }
    
    return cPath;
}


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
std::string environment::open_mime() {
#ifdef __linux__    

        // ------------------------------------------------------------
        // LINUX implemententation

        return "xdg-open";
#endif    

#ifdef __WIN32__

        // ------------------------------------------------------------
        // WINDOWS implemententation

        return "start";
#endif    
}


/**
 * retrieves the path to the system's temp path
 *
 * @return  the path to the system's temp path
 */
boost::filesystem::path environment::temp_path() {
    return boost::filesystem::temp_directory_path();
}

