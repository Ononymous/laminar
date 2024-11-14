cmake_minimum_required(VERSION 3.13)

project(
        LaminarOperationSystem
        VERSION 1.0
        DESCRIPTION "Adds operation support to Laminar"
        LANGUAGES C)

#add_subdirectory(documentation)
add_subdirectory(src)
add_subdirectory(test)