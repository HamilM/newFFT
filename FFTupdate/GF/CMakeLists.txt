cmake_minimum_required(VERSION 3.1)
project(gf_library CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mpclmul --std=c++11")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(CUDA QUIET REQUIRED)
set(GF_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "GF Library Source Dir")
set(GF_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "GF Library Headers")
add_library(gf STATIC
        ${GF_SOURCE_DIR}/F2X.cpp
        ${GF_SOURCE_DIR}/F2XE.cpp
        ${GF_SOURCE_DIR}/GF2.cpp
        ${GF_SOURCE_DIR}/Basis.h
        ${GF_SOURCE_DIR}/LinearizedPoly.h
        ${GF_SOURCE_DIR}/F2XEX.h)
target_include_directories(gf PUBLIC ${CUDA_INCLUDE_DIRS} ${GF_INCLUDE_DIR}) 
ADD_SUBDIRECTORY(Tests)
