cmake_minimum_required(VERSION 2.8.9)
project (name)
set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(name SHARED src/name.c)

install(TARGETS name DESTINATION bin)
install(FILES include/.h DESTINATION include)