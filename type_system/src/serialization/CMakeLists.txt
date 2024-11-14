#set(TS_ERROR_HEADER_LIST "${CSPOTTypeSystem_SOURCE_DIR}/include/error/type_system/types/ts_error.h")
add_library(ts_native_serialization
        ts_native_serialization.c
        ts_native_serialization.h
        ts_serialization_util.c)
#target_include_directories(ts_error PUBLIC ${CSPOTTypeSystem_SOURCE_DIR}/include/error)
target_link_libraries(ts_native_serialization
        PRIVATE ts_type)

#set(TS_ERROR_HEADER_LIST "${CSPOTTypeSystem_SOURCE_DIR}/include/error/type_system/types/ts_error.h")
add_library(ts_string_serialization
        ts_string_serialization.c
        ts_string_serialization.h)
#target_include_directories(ts_error PUBLIC ${CSPOTTypeSystem_SOURCE_DIR}/include/error)
target_link_libraries(ts_string_serialization
        PRIVATE ts_type)