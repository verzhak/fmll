
cmake_minimum_required(VERSION 2.8)
project(fmll C)

include_directories(include lib nn)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pipe -fPIC" CACHE string "" FORCE)

if(NOT CMAKE_BUILD_TYPE)

	# Цель по умолчанию - Debug
	# Для оптимизированной цели: cmake -DCMAKE_BUILD_TYPE=Release ..

	set(CMAKE_BUILD_TYPE Debug)

endif()

add_subdirectory(examples examples)

add_library(fmll SHARED include/test.h lib/all.h nn/test.c)

