
project(examples)

find_package(OpenCV REQUIRED)

pkg_check_modules(PC_GSL QUIET gsl)
string(REPLACE ";" " " PC_GSL_CFLAGS " ${PC_GSL_CFLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PC_GSL_CFLAGS} -Werror -pipe" CACHE string "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pipe -std=c++11" CACHE string "" FORCE)

if(NOT CMAKE_BUILD_TYPE)

	set(CMAKE_BUILD_TYPE Debug)

endif()

add_executable(ex_ff ex_ff.c)
add_executable(ex_mlp ex_mlp.c)
add_executable(ex_som ex_som.c)
add_executable(ex_pca ex_pca.c)
add_executable(ex_pca_2 ex_pca_2.cpp)
#add_executable(ex_captcha ex_captcha.cpp captcha.cpp)
add_executable(ex_svm ex_svm.c)
add_executable(ex_svm_net ex_svm_net.c)
add_executable(ex_random ex_random.c)
add_executable(ex_rbm ex_rbm.cpp)
add_executable(ex_cnn ex_cnn.cpp)

target_link_libraries(ex_ff fmll_ann_ff ${OpenCV_LIBS})
target_link_libraries(ex_mlp fmll_ann_mlp ${OpenCV_LIBS})
target_link_libraries(ex_som fmll_ann_som fmll_math_various ${OpenCV_LIBS})
target_link_libraries(ex_pca fmll_ann_pca fmll_math_various ${PC_GSL_LIBRARIES})
target_link_libraries(ex_pca_2 fmll_ann_pca fmll_math_various ${OpenCV_LIBS})
#target_link_libraries(ex_captcha fmll_ann_pca fmll_math_various ${OpenCV_LIBS})
target_link_libraries(ex_svm fmll_ann_svm ${OpenCV_LIBS})
target_link_libraries(ex_svm_net fmll_ann_svm ${OpenCV_LIBS})
target_link_libraries(ex_random fmll_math_random)
target_link_libraries(ex_rbm fmll_ann_rbm ${OpenCV_LIBS})
target_link_libraries(ex_cnn fmll_ann_cnn ${OpenCV_LIBS})

