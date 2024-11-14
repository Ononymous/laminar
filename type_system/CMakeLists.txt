cmake_minimum_required(VERSION 3.13)

project(
        CSPOTTypeSystem
        VERSION 1.0
        DESCRIPTION "Adds type support to CSPOT"
        LANGUAGES C)

if(DEVICE MATCHES NO)
find_package(woof REQUIRED)
add_subdirectory(documentation)
add_subdirectory(test)
endif()

add_subdirectory(src)
add_subdirectory(extern)
