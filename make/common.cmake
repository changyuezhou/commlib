cmake_minimum_required(VERSION 2.8.0)

option(M64 "Generate 64 bits program" off)
if(M64)
  set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}   -m64 -fPIC -std=c++11")
  set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -m64 -fPIC -std=c++11")
else()
  set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}   -m32 -march=i686 -std=c++11")
  set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -m32 -march=i686 -std=c++11")
endif()