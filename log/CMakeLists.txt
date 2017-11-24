cmake_minimum_required(VERSION 2.8.0)

project(log)

AUX_SOURCE_DIRECTORY(./inc DIR_LOG_INC)
AUX_SOURCE_DIRECTORY(./src DIR_LOG_SRC)

add_library(log STATIC ${DIR_LOG_INC} ${DIR_LOG_SRC})

add_subdirectory(test)
