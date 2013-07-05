# ------------------------------------------------------------
# CMakeLists.txt the AIT Wirecloud Webkit native-client
# 
# This work is part of
#
#   CRISMA 
#
#       an Integration project financed under the 
#       Security theme of the Cooperation Programme 
#       of the 7th Framework Programme of 
#       the European Commission
#
# Autor: Oliver Maurhart, <oliver.maurhart@ait.ac.at>
# 
# Copyright (C) 2013, AIT Austrian Institute of Technology
# AIT Austrian Institute of Technology GmbH
# Donau-City-Strasse 1 | 1220 Vienna | Austria
# http://www.ait.ac.at
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation version 2.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, 
# Boston, MA  02110-1301, USA.
# ------------------------------------------------------------

# project data
project(wirecloud-webkit C CXX)
cmake_minimum_required(VERSION 2.6)

# load necessary basic cmake modules
include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckIncludeFiles)
include(CheckLibraryExists)

# enable tests
ENABLE_TESTING()


# ------------------------------------------------------------
# set global compiler flags

set(VERSION "0.2")
add_definitions(-DVERSION=\"${VERSION}\")

# we relay on a GNU/BSD SOURCE
add_definitions(-D_GNU_SOURCE)
add_definitions(-D_BSD_SOURCE)

# set compile flags
if (CMAKE_COMPILER_IS_GNUCC)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Wextra -pedantic -g -ggdb3 -rdynamic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -Wall -Wextra -pedantic -g -ggdb3 -rdynamic")
    
    # more strict on pure UNIX gcc/g++
    if (NOT CMAKE_COMPILER_IS_MINGW)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    endif (NOT CMAKE_COMPILER_IS_MINGW)
    
endif (CMAKE_COMPILER_IS_GNUCC)

option(DEBUG_MODE_ENABLED "enable debug mode" off)
if (CMAKE_COMPILER_IS_GNUCC)
    if (DEBUG_MODE_ENABLED)
        message("debug and profiling mode enabled")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    endif(DEBUG_MODE_ENABLED)
endif (CMAKE_COMPILER_IS_GNUCC)


# ------------------------------------------------------------
# check libs and packages (headers + lib)

# standard C files
check_include_file(stdio.h HAVE_STDIO_H)
check_include_file(stddef.h HAVE_STDDEF_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(memory.h HAVE_MEMORY_H)
check_include_file(string.h HAVE_STRING_H)
check_include_file(unistd.h HAVE_UNISTD_H)

# time
check_include_file(sys/time.h HAVE_SYS_TIME_H)
check_include_file(time.h HAVE_TIME_H)

# file system stuff
check_include_file(fcntl.h HAVE_FCNTL_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)

# math
check_include_file(math.h HAVE_MATH_H)

# stdbool
check_include_file(stdbool.h HAVE_STDBOOL_H)

# endian
check_include_file(endian.h HAVE_ENDIAN_H)

# math.h
check_include_file(math.h HAVE_MATH_H)

# networking
check_include_file(netdb.h HAVE_NETDB_H)
check_include_file(ifaddrs.h HAVE_IFADDRS_H)
check_include_file(netinet/in.h HAVE_NETINET_IN_H)
check_include_file(arpa/inet.h HAVE_ARPA_INET_H)
check_include_file(sys/socket.h HAVE_SYS_SOCKET_H)
check_include_file(sys/un.h HAVE_SYS_UN_H)

# windows networking
check_include_file(Winsock2.h HAVE_WINSOCK2_H)

# assert
check_include_file(assert.h HAVE_ASSERT_H)

# signal
check_include_file(signal.h HAVE_SIGNAL_H)

# sys/uio
check_include_file(sys/uio.h HAVE_SYS_UIO_H)

# syslog
check_include_file(syslog.h HAVE_SYSLOG_H)

# errno
check_include_file(errno.h HAVE_ERRNO_H)

# limits
check_include_file(limits.h HAVE_LIMITS_H)

# sys/mman.h
check_include_file(sys/mman.h HAVE_SYS_MMAN_H)

# get BOOST!
# uncomment the lines below to fix a certain boost path
# set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/boost")
# set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/boost/lib")
if (WIN32)
    find_package(Boost 1.49.0)
else (WIN32)
    find_package(Boost 1.49.0 COMPONENTS filesystem program_options system)
endif (WIN32)
if (Boost_FOUND)
    set(CMAKE_REQUIRED_LIBRARIES "${Boost_LIBRARIES};${CMAKE_REQUIRED_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
    set(HAVE_BOOST_LIB 1)
else (Boost_FOUND)
    message(FATAL_ERROR "Boost >= 1.49.0 library not found.")
endif (Boost_FOUND)

# Qt4
find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork QtScript QtWebKit)
if (NOT QT4_FOUND)
    message(FATAL_ERROR "Qt4 not found! Is QTDIR environment variable set?")
else (NOT QT4_FOUND)
    include(${QT_USE_FILE})
    set(CMAKE_REQUIRED_LIBRARIES "${QT_LIBRARIES};${CMAKE_REQUIRED_LIBRARIES}")
    set(HAVE_QT4_PKG 1)
endif (NOT QT4_FOUND)


# ------------------------------------------------------------
# some windows fixes

set(ENFORCE_GUI "")
if (WIN32)

    # add winsock
    if (HAVE_WINSOCK2_H)
        set(CMAKE_REQUIRED_LIBRARIES "Ws2_32.lib;${CMAKE_REQUIRED_LIBRARIES}")
    endif (HAVE_WINSOCK2_H)
    
    # fix for GUI applications
    set(ENFORCE_GUI "WIN32")
    set(CMAKE_REQUIRED_LIBRARIES "${QT_QTMAIN_LIBRARY};${CMAKE_REQUIRED_LIBRARIES}")
    
endif (WIN32)


# ------------------------------------------------------------
# dump the config file

# create the config.h
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})


# ------------------------------------------------------------
# go through the subs

add_subdirectory(bin)


# ------------------------------------------------------------
# packaging

set(CPACK_PACKAGE_NAME "wirecloud-webkit")

set(CPACK_PACKAGE_DESCRIPTION "WirecloudWebkit")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "This is the native demo client to show Wirecloud Webkit interaction.")
set(CPACK_PACKAGE_CONTACT "Oliver Maurhart <oliver.maurhart@ait.ac.at>")
set(CPACK_PACKAGE_VENDOR "AIT")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PROJECT_VERSION_STRING "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )

set(CPACK_GENERATOR "DEB;RPM;")
set(CPACK_SOURCE_GENERATOR "TGZ")

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}-${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
set(CPACK_SOURCE_IGNORE_FILES "/build/*;/.git/;/.gitignore")

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++6, libqt4-core (>= 4.6), libqt4-webkit (>= 4.6),  libboost-filesystem1.49.0, libboost-program-options1.49.0, libboost-system1.49.0")

# debianization
string(TOLOWER "${CPACK_PACKAGE_NAME}" CPACK_PACKAGE_NAME_LOWERCASE)
find_program(DPKG_PROGRAM dpkg DOC "dpkg program of Debian-based systems")
if (DPKG_PROGRAM)

    # use dpkg to fix the package file name
    execute_process(
        COMMAND ${DPKG_PROGRAM} --print-architecture
        OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME_LOWERCASE}_${CPACK_PROJECT_VERSION_STRING}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
    
else (DPKG_PROGRAM)
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME_LOWERCASE}_${CPACK_PROJECT_VERSION_STRING}_${CMAKE_SYSTEM_NAME}")
endif (DPKG_PROGRAM)

# package it
include(CPack)
