cmake_minimum_required(VERSION 2.8.0)

project(logTest)

add_executable(log_test ./main.cc)

TARGET_LINK_LIBRARIES(log_test log magic etc pthread dl) 
