
project(examples)

find_package(OpenCV REQUIRED)

if(NOT CMAKE_BUILD_TYPE)

	# Цель по умолчанию - Debug
	# Для оптимизированной цели: cmake -DCMAKE_BUILD_TYPE=Release ..

	set(CMAKE_BUILD_TYPE Debug)

endif()

add_executable(ex_som ex_som.c)
target_link_libraries(ex_som fmll)
target_link_libraries(ex_som ${OpenCV_LIBS})

add_executable(ex_perceptron ex_perceptron.c)
target_link_libraries(ex_perceptron fmll)
target_link_libraries(ex_perceptron ${OpenCV_LIBS})
target_link_libraries(ex_perceptron ${PC_GSL_LIBRARIES})

add_executable(ex_pca ex_pca.c)
target_link_libraries(ex_pca fmll)
target_link_libraries(ex_pca ${PC_GSL_LIBRARIES})

