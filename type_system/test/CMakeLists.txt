include(FetchContent)
fetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.3.2
)
fetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

add_executable(ts_test_print
        ts_test_print.cpp)
target_link_libraries(ts_test_print
        PRIVATE Catch2::Catch2WithMain
        PRIVATE ts_type)
catch_discover_tests(ts_test_print)
target_compile_features(df_test_arithmetic PUBLIC cxx_std_17)

