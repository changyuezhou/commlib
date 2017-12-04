cmake_minimum_required(VERSION 2.8.0)

project(dbTest)

ADD_EXECUTABLE(db_test ./main.cc)

LINK_DIRECTORIES(/usr/lib64/mysql/)

TARGET_LINK_LIBRARIES(db_test db_lib log magic etc cache_lib thread pthread dl mysqlclient)
