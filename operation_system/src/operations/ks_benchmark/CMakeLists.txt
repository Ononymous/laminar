include(FetchContent) # If not included already

FetchContent_Declare(sciplot_content
        GIT_REPOSITORY https://github.com/sciplot/sciplot.git
        GIT_TAG master)

FetchContent_GetProperties(sciplot_content)
if(NOT sciplot_content_POPULATED)
    FetchContent_Populate(sciplot_content)
endif()


file(GLOB KT_SRC "*.cpp")

add_library(ks_test ${KT_SRC})
target_link_libraries(ks_test
	PRIVATE euca_utils
        PRIVATE df_operation
        PRIVATE df_logger
        PRIVATE ts_type)

target_include_directories(ks_test PRIVATE ${sciplot_content_SOURCE_DIR}
				   PRIVATE deps/euca_cutils)
target_compile_features(ks_test PUBLIC cxx_std_17)
