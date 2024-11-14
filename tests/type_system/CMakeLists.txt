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

add_executable(ts_test_array
        ts_test_array.cpp)
target_link_libraries(ts_test_array
        PRIVATE Catch2::Catch2WithMain
        PRIVATE ts_type
        PRIVATE ts_array
        PRIVATE df_interface)
catch_discover_tests(ts_test_array)
target_compile_features(ts_test_array PUBLIC cxx_std_17)

add_executable(ts_test_prim_2d_array ts_test_prim_2d_array.cpp)
target_link_libraries(ts_test_prim_2d_array
        PRIVATE df_interface
        PRIVATE ts_type)