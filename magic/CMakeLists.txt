cmake_minimum_required(VERSION 2.8.0)

project(magic)

AUX_SOURCE_DIRECTORY(./inc DIR_MAGIC_INC)
AUX_SOURCE_DIRECTORY(./src DIR_MAGIC_SRC)

add_library(magic STATIC ${DIR_MAGIC_INC} ${DIR_MAGIC_SRC})
