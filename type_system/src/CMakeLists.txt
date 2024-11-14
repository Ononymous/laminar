add_subdirectory(converter)
add_subdirectory(serialization)
add_subdirectory(types)

set(TS_MAIN_HEADER_LIST
        "${CSPOTTypeSystem_SOURCE_DIR}/include/type_system/type_system/ts_settings.h"
        "${CSPOTTypeSystem_SOURCE_DIR}/include/type_system/type_system/ts_type.h"
        "${CSPOTTypeSystem_SOURCE_DIR}/include/type_system/type_system/ts_types.h")

add_library(ts_type
        ts_type.c
        ts_types.c
        ${TS_MAIN_HEADER_LIST})
add_library(CSPOT::ts_type ALIAS ts_type)
target_include_directories(ts_type PUBLIC ../include/type_system)
target_link_libraries(ts_type
        PUBLIC ts_array
        PUBLIC ts_error
        PUBLIC ts_list
        PUBLIC ts_matrix
        PUBLIC ts_primitive
        PUBLIC ts_string
        PRIVATE uuid
        PRIVATE woof)