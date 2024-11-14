include(FetchContent) # If not included already

FetchContent_Declare(sciplot_content
        GIT_REPOSITORY https://github.com/sciplot/sciplot.git
        GIT_TAG master)

FetchContent_GetProperties(sciplot_content)
if(NOT sciplot_content_POPULATED)
    FetchContent_Populate(sciplot_content)
endif()


file(GLOB SRC "*.cpp")

add_library(mapreduce ${SRC})
target_link_libraries(mapreduce
        PRIVATE df_operation
        PRIVATE df_logger
        PRIVATE ts_type)
target_include_directories(mapreduce PRIVATE ${sciplot_content_SOURCE_DIR})
target_compile_features(mapreduce PUBLIC cxx_std_17)