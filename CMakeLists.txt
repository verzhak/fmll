
cmake_minimum_required(VERSION 2.8)
project(fmll C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pipe -fPIC" CACHE string "" FORCE)

if(NOT CMAKE_BUILD_TYPE)

	# Цель по умолчанию - Debug
	# Для оптимизированной цели: cmake -DCMAKE_BUILD_TYPE=Release ..

	set(CMAKE_BUILD_TYPE Debug)

endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")

	set(DEBUG 1)

endif()

include_directories(include lib nn)
configure_file(lib/all.h.in all.h)

add_subdirectory(examples examples)

add_library(fmll SHARED include/test.h lib/all.h nn/test.c)

