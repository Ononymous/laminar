set(TS_ERROR_HEADER_LIST "${CSPOTTypeSystem_SOURCE_DIR}/include/error/type_system/types/ts_error.h")
add_library(ts_error
        ts_error.c
        ${TS_ERROR_HEADER_LIST})
target_include_directories(ts_error PUBLIC ${CSPOTTypeSystem_SOURCE_DIR}/include/error)
target_link_libraries(ts_error
        PRIVATE ts_type)

set(TS_PRIMITIVE_HEADER_LIST "${CSPOTTypeSystem_SOURCE_DIR}/include/primitive/type_system/types/ts_primitive.h")
add_library(ts_primitive
        ts_primitive.c
        ${TS_PRIMITIVE_HEADER_LIST})
target_include_directories(ts_primitive PUBLIC ${CSPOTTypeSystem_SOURCE_DIR}/include/primitive)
target_link_libraries(ts_primitive
        PRIVATE ts_type)

set(TS_MATRIX_HEADER_LIST "${CSPOTTypeSystem_SOURCE_DIR}/include/matrix/type_system/types/ts_matrix.h")
add_library(ts_matrix
        ts_matrix.c
        ${TS_MATRIX_HEADER_LIST})
target_include_directories(ts_matrix PUBLIC ${CSPOTTypeSystem_SOURCE_DIR}/include/matrix)
target_link_libraries(ts_matrix
        PRIVATE ts_type
        PRIVATE uuid)

set(TS_ARRAY_HEADER_LIST "${CSPOTTypeSystem_SOURCE_DIR}/include/array/type_system/types/ts_array.h")
add_library(ts_array
        ts_array.c
        ${TS_ARRAY_HEADER_LIST})
target_include_directories(ts_array PUBLIC ${CSPOTTypeSystem_SOURCE_DIR}/include/array)
target_link_libraries(ts_array
        PRIVATE ts_type
        PRIVATE uuid)

set(TS_LIST_HEADER_LIST "${CSPOTTypeSystem_SOURCE_DIR}/include/list/type_system/types/ts_list.h")
add_library(ts_list
        ts_list.c
        ${TS_LIST_HEADER_LIST})
target_include_directories(ts_list PUBLIC ${CSPOTTypeSystem_SOURCE_DIR}/include/list)
target_link_libraries(ts_list
        PRIVATE ts_type
        PRIVATE uuid)

set(TS_STRING_HEADER_LIST "${CSPOTTypeSystem_SOURCE_DIR}/include/string/type_system/types/ts_string.h")
add_library(ts_string
        ts_string.c
        ${TS_STRING_HEADER_LIST})
target_include_directories(ts_string PUBLIC ${CSPOTTypeSystem_SOURCE_DIR}/include/string)
target_link_libraries(ts_string
        PRIVATE ts_type)