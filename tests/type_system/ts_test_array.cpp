//
// Created by Lukas Brand on 03.06.2023.
//
#include "../../df_interface.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("array") {
    system("sudo rm -f lmr*");
    laminar_reset(); /* reset setup data structures */
    set_host(1);
    add_host(1, "127.0.0.1", "/home/centos/cspot/build/bin/");
    laminar_init();


    SECTION("DIFFERENT->0") {

        // REQUIRE(return_value == 1);
    } //
}
