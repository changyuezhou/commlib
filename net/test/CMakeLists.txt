cmake_minimum_required(VERSION 2.8.0)

project(netTest)

#ice_add_executable(netTest
#  ./test.cc
#  DEPENDS net gtest gtest_main
#  LINK_LIBS net gtest gtest_main
#)

ice_add_executable(service
  ./service.cc
  DEPENDS net
  LINK_LIBS net
)

ice_add_executable(dgram_service
  ./dgram_service.cc
  DEPENDS net
  LINK_LIBS net
)

ice_add_executable(agent
  ./agent.cc
  DEPENDS net
  LINK_LIBS net
)
