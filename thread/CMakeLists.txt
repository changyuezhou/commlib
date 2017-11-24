cmake_minimum_required(VERSION 2.8.0)

project(thread)

AUX_SOURCE_DIRECTORY(./inc DIR_THREAD_INC)
AUX_SOURCE_DIRECTORY(./src DIR_THREAD_SRC)

add_library(thread ${DIR_THREAD_INC} ${DIR_THREAD_SRC})

#add_subdirectory(test)
