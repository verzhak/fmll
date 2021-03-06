
cmake_minimum_required(VERSION 2.8)

#
# Кросскомпиляция для Windows в Linux с помощью MinGW: cmake -DWindows=1 ..
#
# Для генерации lib - файлов, необходимых для связывания с библиотекой проектов, разрабатываемых в Visual Studio,
# после сборки dll'ок необходимо выполнить скрипт ../various/dll/create_lib.sh (вызывать данный скрипт можно только (!) из каталога build)
#

if(Windows)

	set(CMAKE_C_COMPILER i486-mingw32-gcc)
	set(CMAKE_CXX_COMPILER i486-mingw32-g++)
	set(CMAKE_RC_COMPILER i486-mingw32-windres)
	set(CMAKE_FIND_ROOT_PATH /usr/i486-mingw32) # Только для ArchLinux
	set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
	set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
	set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
	set(CMAKE_INSTALL_PREFIX ${CMAKE_FIND_ROOT_PATH})

	add_definitions(-DFMLL_OS_WINDOWS)

endif()

find_package(OpenMP REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(PC_MXML QUIET mxml)

string(REPLACE ";" " " PC_MXML_CFLAGS " ${PC_MXML_CFLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} ${PC_MXML_CFLAGS} -Werror -pipe -ansi -std=c90 -pedantic" CACHE string "" FORCE)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")

	#
	# Цель по умолчанию - Debug
	# Для оптимизированной цели: cmake -DCMAKE_BUILD_TYPE=Release ..
	#

	set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)

endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")

	set(FMLL_DEBUG 1)

endif()

add_definitions(-DFMLL_BUILD)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
configure_file(private/all.h.in all.h)

set(fmll_lib_sources lib/memory.c lib/print.c private/xml.c)
set(fmll_math_graph_sources math/graph/graph.c)
set(fmll_math_matrix_sources math/matrix/matrix.c)
set(fmll_math_random_sources math/random/random.c math/random/mt19937.c math/random/lcg.c)
set(fmll_math_various_sources math/various/distance.c math/various/various.c)
set(fmll_ann_base_sources ann/base/activate.c ann/base/timing.c ann/base/weight_init.c)
set(fmll_ann_ff_sources ann/ff/ff.c)
set(fmll_ann_mlp_sources ann/mlp/mlp.c)
set(fmll_ann_som_sources ann/som/som.c)
set(fmll_ann_pca_sources ann/pca/pca.c)
set(fmll_ann_svm_sources ann/svm/kernel.c ann/svm/svm.c ann/svm/svm_net.c)
set(fmll_ann_rbm_sources ann/rbm/rbm.c)
set(fmll_ann_cnn_sources ann/cnn/cnn.c)

add_library(fmll_lib SHARED ${fmll_lib_sources})
add_library(fmll_math_graph SHARED ${fmll_math_graph_sources})
add_library(fmll_math_matrix SHARED ${fmll_math_matrix_sources})
add_library(fmll_math_random SHARED ${fmll_math_random_sources})
add_library(fmll_math_various SHARED ${fmll_math_various_sources})
add_library(fmll_ann_base SHARED ${fmll_ann_base_sources})
add_library(fmll_ann_ff SHARED ${fmll_ann_ff_sources})
add_library(fmll_ann_mlp SHARED ${fmll_ann_mlp_sources})
add_library(fmll_ann_som SHARED ${fmll_ann_som_sources})
add_library(fmll_ann_pca SHARED ${fmll_ann_pca_sources})
add_library(fmll_ann_svm SHARED ${fmll_ann_svm_sources})
add_library(fmll_ann_rbm SHARED ${fmll_ann_rbm_sources})
add_library(fmll_ann_cnn SHARED ${fmll_ann_cnn_sources})

target_link_libraries(fmll_lib ${PC_MXML_LIBRARIES})
target_link_libraries(fmll_math_graph m fmll_lib)
target_link_libraries(fmll_math_matrix m fmll_lib)
target_link_libraries(fmll_math_random m fmll_lib)
target_link_libraries(fmll_math_various m fmll_lib)
target_link_libraries(fmll_ann_base fmll_lib fmll_math_random)
target_link_libraries(fmll_ann_ff fmll_ann_base fmll_math_matrix fmll_math_graph fmll_math_random)
target_link_libraries(fmll_ann_mlp fmll_ann_base fmll_ann_ff fmll_math_matrix fmll_math_random)
target_link_libraries(fmll_ann_som fmll_ann_base fmll_math_matrix)
target_link_libraries(fmll_ann_pca fmll_ann_base fmll_math_matrix)
target_link_libraries(fmll_ann_svm fmll_ann_base)
target_link_libraries(fmll_ann_rbm fmll_ann_base fmll_math_matrix)
target_link_libraries(fmll_ann_cnn fmll_ann_base fmll_math_matrix)

if(Windows)

	set_target_properties(fmll_lib PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_math_graph PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_math_matrix PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_math_random PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_math_various PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_ann_base PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_ann_ff PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_ann_mlp PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_ann_som PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_ann_pca PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_ann_svm PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_ann_rbm PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_ann_cnn PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")

endif()

install(TARGETS fmll_lib LIBRARY DESTINATION lib)
install(TARGETS fmll_math_graph LIBRARY DESTINATION lib)
install(TARGETS fmll_math_matrix LIBRARY DESTINATION lib)
install(TARGETS fmll_math_random LIBRARY DESTINATION lib)
install(TARGETS fmll_math_various LIBRARY DESTINATION lib)
install(TARGETS fmll_ann_base LIBRARY DESTINATION lib)
install(TARGETS fmll_ann_ff LIBRARY DESTINATION lib)
install(TARGETS fmll_ann_mlp LIBRARY DESTINATION lib)
install(TARGETS fmll_ann_som LIBRARY DESTINATION lib)
install(TARGETS fmll_ann_pca LIBRARY DESTINATION lib)
install(TARGETS fmll_ann_svm LIBRARY DESTINATION lib)
install(TARGETS fmll_ann_rbm LIBRARY DESTINATION lib)
install(TARGETS fmll_ann_cnn LIBRARY DESTINATION lib)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/fmll.h DESTINATION include/fmll)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib DESTINATION include/fmll FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/math DESTINATION include/fmll FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ann DESTINATION include/fmll FILES_MATCHING PATTERN "*.h")

if(NOT Windows)

	configure_file(fmll.pc.in fmll.pc)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/fmll.pc DESTINATION lib/pkgconfig)

	add_subdirectory(examples examples)

endif()

