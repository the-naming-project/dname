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
cmake_minimum_required(VERSION 3.9)
#
# libname.so
# 0.0.1
#
project(name VERSION 0.0.1 DESCRIPTION "Deterministic Naming Library")

#
# Literally define the internal source .c files
# here.
#
# Please list these manually within each pull request.
#
# cd src && ls -l *.c
add_library(name SHARED
        src/name.c
        src/version.h
        )

#
# Version of the library properties
#
set_target_properties(name PROPERTIES VERSION ${PROJECT_VERSION})
#
# This is where the magic symlinking in /usr/include comes to play
#  - libname.1
#  - libname.0.0.1
#
set_target_properties(name PROPERTIES SOVERSION 0.0.1)

#
# Public API
#
# Everything in /include will be what we expose to a third party
# application.
set_target_properties(name PROPERTIES PUBLIC_HEADER include/name.h)

# Add the top level directory to make our #include statements cleaner
target_include_directories(name PRIVATE .)
# Add include (public API)
target_include_directories(name PRIVATE include)
# Add src (private implementation)
target_include_directories(name PRIVATE src)

# make install
# Here is the install rule
include(GNUInstallDirs)
# This is where we put the shared object on the filesystem
install(TARGETS name
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})