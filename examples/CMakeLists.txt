
project(examples)

include_directories(../include)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pipe" CACHE string "" FORCE)

if(NOT CMAKE_BUILD_TYPE)

	# Цель по умолчанию - Debug
	# Для оптимизированной цели: cmake -DCMAKE_BUILD_TYPE=Release ..

	set(CMAKE_BUILD_TYPE Debug)

endif()

add_executable(ex_test ex_test.c)
target_link_libraries(ex_test fmll)

