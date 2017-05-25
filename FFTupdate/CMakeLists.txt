cmake_minimum_required (VERSION 2.6)
project (newFFT C CXX)
set(CMAKE_CXX_STANDARD 11 CACHE INTERNAL "C++ Standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE INTERNAL "C++ Standard Turned on")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
ADD_SUBDIRECTORY(googletest-master)
ADD_SUBDIRECTORY(GF)

#  add_executable(newFFT main.cu)
