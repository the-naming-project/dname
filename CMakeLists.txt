# Copyright © 2021 Kris Nóva <kris@nivenly.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

###############################
set(DNAME_SO_VERSION "0.1.1") #
###############################




cmake_minimum_required(VERSION 3.9)
project(dname VERSION ${DNAME_SO_VERSION} DESCRIPTION "Deterministic Naming Library")

#
# Literally define the internal source .c files
# here.
#
# Please list these manually within each pull request.
#
# cd src && ls -l *.c
add_library(dname SHARED
        src/bijective.c
        src/linux.c
        src/dname.c
        src/names.c
        src/bijective.h
        src/linux.h
        src/names.h
        src/version.h
        )

find_package(OpenSSL REQUIRED)
target_link_libraries(dname OpenSSL::SSL)

#
# Version of the library properties
#
set_target_properties(dname PROPERTIES VERSION ${DNAME_SO_VERSION})
#
# This is where the magic symlinking in /usr/include comes to play
set_target_properties(dname PROPERTIES SOVERSION ${DNAME_SO_VERSION})

#
# Public API
#
# Everything in /include will be what we expose to a third party
# application.
set_target_properties(dname PROPERTIES PUBLIC_HEADER include/dname.h)

# Add the top level directory to make our #include statements cleaner
target_include_directories(dname PRIVATE .)
# Add include (public API)
target_include_directories(dname PRIVATE include)
# Add src (private implementation)
target_include_directories(dname PRIVATE src)

# make install
# Here is the install rule
include(GNUInstallDirs)
# This is where we put the shared object on the filesystem
install(TARGETS dname
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
