cmake_minimum_required(VERSION 2.8.0)

project(threadTest)

ice_add_executable(thread_test
  ./main_thread.cc
  DEPENDS thread
  LINK_LIBS log etc magic thread pthread
)

ice_add_executable(process_test
  ./main_process.cc
  DEPENDS thread
  LINK_LIBS log etc magic thread pthread
)