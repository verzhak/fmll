
project(examples)

find_package(OpenCV REQUIRED)

pkg_check_modules(PC_GSL QUIET gsl)
string(REPLACE ";" " " PC_GSL_CFLAGS " ${PC_GSL_CFLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PC_GSL_CFLAGS} -Wall -pipe" CACHE string "" FORCE)

if(NOT CMAKE_BUILD_TYPE)

	# Цель по умолчанию - Debug
	# Для оптимизированной цели: cmake -DCMAKE_BUILD_TYPE=Release ..

	set(CMAKE_BUILD_TYPE Debug)

endif()

add_executable(ex_perceptron ex_perceptron.c)
add_executable(ex_som ex_som.c)
add_executable(ex_pca ex_pca.c)
add_executable(ex_svm ex_svm.c)
add_executable(ex_svm_net ex_svm_net.c)

target_link_libraries(ex_perceptron fmll_nn_perceptron ${OpenCV_LIBS})
target_link_libraries(ex_som fmll_nn_som ${OpenCV_LIBS})
target_link_libraries(ex_pca fmll_nn_pca ${PC_GSL_LIBRARIES})
target_link_libraries(ex_svm fmll_nn_svm ${OpenCV_LIBS})
target_link_libraries(ex_svm_net fmll_nn_svm ${OpenCV_LIBS})

