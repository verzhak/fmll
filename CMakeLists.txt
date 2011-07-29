
cmake_minimum_required(VERSION 2.8)

find_package(OpenMP REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -Wall -pipe -fPIC" CACHE string "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm" CACHE string "" FORCE)

if(NOT CMAKE_BUILD_TYPE)

	# Цель по умолчанию - Debug
	# Для оптимизированной цели: cmake -DCMAKE_BUILD_TYPE=Release ..

	set(CMAKE_BUILD_TYPE Debug)

endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")

	set(DEBUG 1)

endif()

include_directories(include lib nn build)
configure_file(include/all.h.in all.h)

add_subdirectory(examples examples)

add_library(fmll SHARED include/all.h include/exception.h include/memory.h lib/memory.c include/activate.h nn/activate.c include/distance.h nn/distance.c include/weight_init.h nn/weight_init.c include/timing.h nn/timing.c include/various.h nn/various.c include/perceptron.h nn/perceptron.c include/som.h nn/som.c)
target_link_libraries(fmll gsl gslcblas)

