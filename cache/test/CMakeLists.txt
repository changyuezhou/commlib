cmake_minimum_required(VERSION 2.8.0)

project(ptmallocTest)

ice_add_executable(ptmallocTest
        ./ptmalloc_test.cc
        DEPENDS log magic etc cache_lib
        LINK_LIBS log magic etc cache_lib pthread
        )

ice_add_executable(node_group_test
        ./node_group_test.cc
        DEPENDS log magic etc cache_lib
        LINK_LIBS log magic etc thread cache_lib pthread ext_lib
        )

ice_add_executable(index_group_test
        ./index_group_test.cc
        DEPENDS log magic etc cache_lib
        LINK_LIBS log magic etc thread cache_lib pthread ext_lib
        )

ice_add_executable(rbtree_test
        ./rbtree_test.cc
        DEPENDS log magic etc cache_lib
        LINK_LIBS log magic etc cache_lib pthread ext_lib
        )