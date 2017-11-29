cmake_minimum_required(VERSION 2.8.0)

project(dbTest)

ice_add_executable(db_test
        ./main.cc
        DEPENDS log magic etc cache_lib db ext_lib
        LINK_LIBS ext_lib log magic etc cache_lib db thread pthread dl mysqlclient
        )