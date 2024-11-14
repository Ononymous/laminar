add_subdirectory(operations)

set(DF_OPERATION_MAIN_HEADER_LIST
        "${LaminarOperationSystem_SOURCE_DIR}/include/operation_system/df_operation.h"
        "${LaminarOperationSystem_SOURCE_DIR}/include/operation_system/df_operations.h")

add_library(df_operation
        df_operation.c
        df_operations.c
        ${DF_OPERATION_MAIN_HEADER_LIST})
add_library(Laminar::df_operation ALIAS df_operation)
target_include_directories(df_operation PUBLIC ${LaminarOperationSystem_SOURCE_DIR}/include)
target_link_libraries(df_operation
        PRIVATE df_logger
        PRIVATE ts_type
        PRIVATE df_operation_arithmetic
        PRIVATE df_operation_cast
        PRIVATE df_operation_internal
        PRIVATE df_operation_linear_algebra
        PRIVATE df_operation_logic
        PRIVATE df_operation_machine_learning
        PRIVATE df_operation_custom_operations
        PRIVATE df_operation_string
        PRIVATE riot
        PRIVATE mapreduce 
	PRIVATE ks_test
	PRIVATE regress_test
	PRIVATE corr_test
	PRIVATE anomaly_test)
