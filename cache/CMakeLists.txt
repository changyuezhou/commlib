cmake_minimum_required(VERSION 2.8.0)

project(cache_lib)

AUX_SOURCE_DIRECTORY(./inc DIR_CACHE_INC)
AUX_SOURCE_DIRECTORY(./src DIR_CACHE_SRC)

add_library(cache_lib STATIC ${DIR_CACHE_INC} ${DIR_CACHE_SRC})

#add_subdirectory(test)
