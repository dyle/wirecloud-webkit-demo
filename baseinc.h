/*
 * baseinc.h
 *
 * Standard header include file to get the most common system definitions
 *
 * Autor: Oliver Maurhart, <oliver.maurhart@ait.ac.at>
 *
 * Copyright (C) 2012, AIT Austrian Institute of Technology
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

#ifndef __BASEINC_H
#define __BASEINC_H

// get definitions found by cmake
#include "config.h"

// ------------------------------------------------------------
// C++

// C++11 standard and boost stuff
#ifdef __cplusplus
#   include <algorithm>
#   include <cassert>
#   include <chrono>
#   include <condition_variable>
#   include <exception>
#   include <fstream>
#   include <iostream>
#   include <iomanip>
#   include <limits>
#   include <list>
#   include <map>
#   include <memory>
#   include <mutex>
#   include <set>
#   include <stdexcept>
#   include <sstream>
#   include <string>
#   include <thread>
#   include <tuple>
#   include <vector>

#   ifdef HAVE_BOOST_LIB
#       include <boost/version.hpp>
#       include <boost/algorithm/string.hpp>
#       include <boost/crc.hpp>
#       include <boost/filesystem.hpp>
#       include <boost/format.hpp>

// fix for boost 1.51: this boost version has some poor programming quality
#   if __GNUC__ == 4 && __GNUC_MINOR__ >= 6 && BOOST_VERSION == 105100
#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wunused-parameter"
#       include <boost/program_options.hpp>
#       pragma GCC diagnostic pop
#   else
#       include <boost/program_options.hpp>
#   endif

#       include <boost/program_options/detail/config_file.hpp>
#       include <boost/range.hpp>
#       include <boost/tokenizer.hpp>
#   endif

#endif


// ------------------------------------------------------------
// check defs (headers only)

// standard C headers
#ifdef HAVE_STDIO_H 
#   include <stdio.h>
#endif

#ifdef HAVE_STDDEF_H 
#   include <stddef.h>
#endif

#ifdef HAVE_STDLIB_H
#   include <stdlib.h>
#endif

#ifdef HAVE_INTTYPES_H
#   include <inttypes.h>
#endif

#ifdef HAVE_MEMORY_H
#   include <memory.h>
#endif

#ifdef HAVE_STRING_H
#   include <string.h>
#endif

#ifdef HAVE_UNISTD_H
#   include <unistd.h>
#endif


// stdbool.h
#ifdef HAVE_STDBOOL_H
#   include <stdbool.h>
#endif


// endian.h
#ifdef HAVE_ENDIAN_H
#   include <endian.h>
#endif


// time system headers
#ifdef HAVE_SYS_TIME_H
#   include <sys/time.h>
#endif

#ifdef HAVE_SYS_TIMES_H
#   include <sys/times.h>
#endif

#ifdef HAVE_TIME_H
#   include <time.h>
#endif


// files
#ifdef HAVE_FCNTL_H
#   include <fcntl.h>
#endif

#ifdef HAVE_SYS_STAT_H
#   include <sys/stat.h>
#endif


// some math
#ifdef HAVE_MATH_H
#   include <math.h>
#endif


// networking
#ifdef HAVE_NETDB_H
#   include <netdb.h>
#endif

#ifdef HAVE_IFADDRS_H
#   include <ifaddrs.h>
#endif

#ifdef HAVE_NETINET_IN_H
#   include <netinet/in.h>
#endif

#ifdef HAVE_ARPA_INET_H
#   include <arpa/inet.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#   include <sys/socket.h>
#endif

#ifdef HAVE_SYS_UN_H
#   include <sys/un.h>
#endif


// Windows networking
#ifdef HAVE_WINSOCK2_H
#   include <Winsock2.h>
#endif


// assert.h
#ifdef HAVE_ASSERT_H
#   include <assert.h>
#endif


// signal.h
#ifdef HAVE_SIGNAL_H
#   include <signal.h>
#endif


// sys/uio.h
#ifdef HAVE_SYS_UIO_H
#   include <sys/uio.h>
#endif


// syslog.h
#ifdef HAVE_SYSLOG_H
#   include <syslog.h>
#endif


// errno.h
#ifdef HAVE_ERRNO_H
#   include <errno.h>
#endif


// limits.h
#ifdef HAVE_LIMITS_H
#   include <limits.h>
#endif


// sys/mman.h
#ifdef HAVE_SYS_MMAN_H
#   include <sys/mman.h>
#endif


// ------------------------------------------------------------
// packages

// Qt4
#ifdef HAVE_QT4_PKG
#ifdef __cplusplus
    // Qt libs are included on a per file basis
    // but QtGlobal and QDebug should be included allways
#   include <QtGlobal>
#   include <QtCore>
#   include <QDebug>
#endif
#endif 


// ------------------------------------------------------------
// Windows

#ifdef _WIN32
#   define __WIN32__
#endif

#ifdef __WIN32__
#   include <windows.h>
#endif


// ------------------------------------------------------------
// Linux

#ifdef __linux__
#   include <execinfo.h>
#endif


// ------------------------------------------------------------
// common macros

#if defined(__GNUC__)
#   define UNUSED   __attribute__((unused))
#else
#   define UNUSED
#endif



#endif

