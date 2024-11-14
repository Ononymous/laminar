set(DF_LOGGER_MAIN_HEADER_LIST
        "${LaminarLoggerSystem_SOURCE_DIR}/include/logger_system/df_logger.h"
        "${LaminarLoggerSystem_SOURCE_DIR}/include/logger_system/df_logger_settings.h"
        "${LaminarLoggerSystem_SOURCE_DIR}/include/logger_system/df_operation_logger.h")

add_library(df_logger
        df_logger.c
        df_operation_logger.c
        ${DF_LOGGER_MAIN_HEADER_LIST})
add_library(Laminar::df_logger ALIAS df_logger)
target_include_directories(df_logger PUBLIC ${LaminarLoggerSystem_SOURCE_DIR}/include)
target_link_libraries(df_logger
        PRIVATE uuid
        PRIVATE ts_type)