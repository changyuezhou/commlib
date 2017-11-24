cmake_minimum_required(VERSION 2.8.0)

project(dist_lib)

AUX_SOURCE_DIRECTORY(./inc DIR_DIST_INC)
AUX_SOURCE_DIRECTORY(./src DIR_DIST_SRC)

add_library(dist_lib STATIC ${DIR_DIST_INC} ${DIR_DIST_SRC})

#add_subdirectory(test)
