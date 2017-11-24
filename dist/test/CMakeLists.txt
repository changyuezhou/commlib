cmake_minimum_required(VERSION 2.8.0)

project(electionTest)

ice_add_executable(election_service
        ./election_service_test.cc
        DEPENDS log magic etc dist_lib thread
        LINK_LIBS log magic etc thread dist_lib pthread zookeeper_mt
        )

ice_add_executable(election_cli
        ./election_cli_test.cc
        DEPENDS log magic etc dist_lib thread
        LINK_LIBS log magic etc thread dist_lib pthread zookeeper_mt
        )

