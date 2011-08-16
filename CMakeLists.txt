
cmake_minimum_required(VERSION 2.8)

find_package(OpenMP REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(PC_GSL QUIET gsl)
pkg_check_modules(PC_MXML QUIET mxml)

string(REPLACE ";" " " PC_GSL_CFLAGS " ${PC_GSL_CFLAGS}")
string(REPLACE ";" " " PC_MXML_CFLAGS " ${PC_MXML_CFLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} ${PC_GSL_CFLAGS} ${PC_MXML_CFLAGS} -Wall -pipe" CACHE string "" FORCE)

if(NOT CMAKE_BUILD_TYPE)

	# Цель по умолчанию - Debug
	# Для оптимизированной цели: cmake -DCMAKE_BUILD_TYPE=Release ..

	set(CMAKE_BUILD_TYPE Debug)

endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")

	set(DEBUG 1)

endif()

execute_process(COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/fmll ERROR_QUIET)
include_directories(include lib nn ${CMAKE_CURRENT_BINARY_DIR})
configure_file(lib/all.h.in all.h)

set(fmll_sources lib/memory.c lib/xml.c nn/activate.c nn/distance.c nn/weight_init.c nn/timing.c nn/various.c nn/perceptron.c nn/som.c nn/pca.c nn/kernel.c nn/svm.c nn/svm_net.c)

add_library(fmll SHARED ${fmll_sources})
target_link_libraries(fmll m ${PC_GSL_LIBRARIES} ${PC_MXML_LIBRARIES})

add_library(fmll-static STATIC ${fmll_sources})
target_link_libraries(fmll-static m ${PC_GSL_LIBRARIES} ${PC_MXML_LIBRARIES})
set_target_properties(fmll-static PROPERTIES OUTPUT_NAME "fmll")

add_subdirectory(examples examples)

