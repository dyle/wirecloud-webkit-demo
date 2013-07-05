# ------------------------------------------------------------
# CMakeLists.txt 
# 
# make: wirecloud-webkit/bin
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


# ------------------------------------------------------------
# basic

# additional includes
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/bin)


# ------------------------------------------------------------
# backmeup-gui

# sources
set(WIRECLOUD_WEBKIK_GUI_SRC

    about_dialog.cpp
    debug.cpp
    environment.cpp
    event_sink.cpp
    main.cpp
    main_widget.cpp
    main_window.cpp
)

# Qt4
QT4_GENERATE_MOC(about_dialog.h     ${CMAKE_CURRENT_BINARY_DIR}/about_dialog.moc.cpp)
QT4_GENERATE_MOC(event_sink.h       ${CMAKE_CURRENT_BINARY_DIR}/event_sink.moc.cpp)
QT4_GENERATE_MOC(main_widget.h      ${CMAKE_CURRENT_BINARY_DIR}/main_widget.moc.cpp)
QT4_GENERATE_MOC(main_window.h      ${CMAKE_CURRENT_BINARY_DIR}/main_window.moc.cpp)
set(WIRECLOUD_WEBKIK_GUI_MOC
    
    about_dialog.moc.cpp
    event_sink.moc.cpp
    main_widget.moc.cpp
    main_window.moc.cpp
)


# windows icon
set(WIRECLOUD_WEBKIT_RES_OBJ "")
if (WIN32)
    ADD_CUSTOM_COMMAND(
        OUTPUT wirecloud-webkit.res
        COMMAND rc /v ${CMAKE_CURRENT_SOURCE_DIR}/wirecloud-webkit.rc
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/wirecloud-webkit.rc
        VERBATIM
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT wirecloud-webkit.res.obj
        COMMAND cvtres /OUT:wirecloud-webkit.res.obj /v wirecloud-webkit.res
        DEPENDS wirecloud-webkit.res
        VERBATIM
    )
     set(WIRECLOUD_WEBKIT_RES_OBJ "wirecloud-webkit.res.obj")
endif (WIN32)


# bin definition
add_executable(wirecloud-webkit ${ENFORCE_GUI} ${WIRECLOUD_WEBKIK_GUI_SRC} ${WIRECLOUD_WEBKIK_GUI_MOC})


# linkage
target_link_libraries(wirecloud-webkit ${CMAKE_REQUIRED_LIBRARIES})


# copy share/qkd-gui stuff for convenient testing
file(COPY ${CMAKE_SOURCE_DIR}/share/wirecloud-webkit DESTINATION ${CMAKE_BINARY_DIR}/share USE_SOURCE_PERMISSIONS)


# install
install(TARGETS wirecloud-webkit RUNTIME DESTINATION bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/share/wirecloud-webkit DESTINATION share)

