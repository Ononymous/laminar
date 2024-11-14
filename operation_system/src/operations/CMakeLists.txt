add_subdirectory(riot_benchmark)
add_subdirectory(ks_benchmark)
add_subdirectory(regress_benchmark)
add_subdirectory(corr_benchmark)
add_subdirectory(anomaly_benchmark)
add_subdirectory(mapreduce_benchmark)

add_library(df_operation_arithmetic
        df_arithmetic.c
        df_arithmetic.h)
target_include_directories(df_operation_arithmetic PRIVATE ${LaminarOperationSystem_SOURCE_DIR}/include)
target_link_libraries(df_operation_arithmetic
        PRIVATE df_logger
        PRIVATE ts_type
        PRIVATE df_operation)

add_library(df_operation_cast
        df_cast.c
        df_cast.h)
target_include_directories(df_operation_cast PRIVATE ${LaminarOperationSystem_SOURCE_DIR}/include)
target_link_libraries(df_operation_cast
        PRIVATE df_logger
        PRIVATE ts_type
        PRIVATE df_operation)

add_library(df_operation_internal
        df_internal.c
        df_internal.h)
target_include_directories(df_operation_internal PRIVATE ${LaminarOperationSystem_SOURCE_DIR}/include)
target_link_libraries(df_operation_internal
        PRIVATE df_logger
        PRIVATE ts_type
        PRIVATE df_operation)

add_library(df_operation_linear_algebra
        df_linear_algebra.c
        df_linear_algebra.h)
target_include_directories(df_operation_linear_algebra PRIVATE ${LaminarOperationSystem_SOURCE_DIR}/include)
target_link_libraries(df_operation_linear_algebra
        PRIVATE df_logger
        PRIVATE ts_type
        PRIVATE df_operation)

add_library(df_operation_logic
        df_logic.c
        df_logic.h)
target_include_directories(df_operation_logic PRIVATE ${LaminarOperationSystem_SOURCE_DIR}/include)
target_link_libraries(df_operation_logic
        PRIVATE df_logger
        PRIVATE ts_type
        PRIVATE df_operation)

add_library(df_operation_string
        df_string.cpp
        df_string.h)
#add_executable(df_operation_string_exe
#        df_string.cpp)
#target_include_directories(df_operation_string_exe PRIVATE ${LaminarOperationSystem_SOURCE_DIR}/include)
#target_link_libraries(df_operation_string_exe
#        PRIVATE df_logger
#        PRIVATE ts_type
#        PRIVATE df_operation)
target_include_directories(df_operation_string PRIVATE ${LaminarOperationSystem_SOURCE_DIR}/include)
target_link_libraries(df_operation_string
        PRIVATE df_logger
        PRIVATE ts_type
        PRIVATE df_operation)

add_library(df_operation_machine_learning
        df_machine_learning.cpp
        df_machine_learning.h)
target_include_directories(df_operation_machine_learning PRIVATE ${LaminarOperationSystem_SOURCE_DIR}/include)
target_link_libraries(df_operation_machine_learning
        PRIVATE df_logger
        PRIVATE ts_type
        PRIVATE df_operation)

add_library(df_operation_custom_operations
        df_custom_operations.cpp
        df_custom_operations.h)
target_include_directories(df_operation_custom_operations PRIVATE ${LaminarOperationSystem_SOURCE_DIR}/include)
target_link_libraries(df_operation_custom_operations
        PRIVATE df_logger
        PRIVATE ts_type
        PRIVATE df_operation)
