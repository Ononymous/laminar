add_executable(primitive_type primitive_type.c)
target_link_libraries(primitive_type
        PRIVATE woof)

add_executable(primitive_type_sys primitive_type_sys.c)
target_link_libraries(primitive_type_sys
        PRIVATE woof
        PRIVATE ts_type)