
cmake_minimum_required(VERSION 2.8)

#
# Кросскомпиляция для Windows в Linux с помощью MinGW: cmake -DWindows=1 ..
#
# Для генерации lib - файлов, необходимых для связывания с библиотекой проектов, разрабатываемых в Visual Studio,
# после сборки dll'ок необходимо выполнить скрипт ../dll/create_lib.sh (вызывать данный скрипт можно только (!) из каталога build)
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

endif()

find_package(OpenMP REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(PC_MXML QUIET mxml)

string(REPLACE ";" " " PC_MXML_CFLAGS " ${PC_MXML_CFLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} ${PC_MXML_CFLAGS} -Wall -pipe" CACHE string "" FORCE)

if(NOT CMAKE_BUILD_TYPE)

	#
	# Цель по умолчанию - Debug
	# Для оптимизированной цели: cmake -DCMAKE_BUILD_TYPE=Release ..
	#

	set(CMAKE_BUILD_TYPE Debug)

endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")

	set(FMLL_DEBUG 1)

endif()

add_definitions(-DFMLL_BUILD)
include_directories(include ${CMAKE_CURRENT_BINARY_DIR})
configure_file(include/private/all.h.in all.h)

set(fmll_lib_sources lib/memory.c lib/xml.c)
set(fmll_math_sources math/matrix.c)
set(fmll_nn_base_sources nn/activate.c nn/distance.c nn/weight_init.c nn/timing.c nn/various.c)
set(fmll_nn_perceptron_sources nn/perceptron.c)
set(fmll_nn_som_sources nn/som.c)
set(fmll_nn_pca_sources nn/pca.c)
set(fmll_nn_svm_sources nn/kernel.c nn/svm.c nn/svm_net.c)

add_library(fmll_lib SHARED ${fmll_lib_sources})
add_library(fmll_math SHARED ${fmll_math_sources})
add_library(fmll_nn_base SHARED ${fmll_nn_base_sources})
add_library(fmll_nn_perceptron SHARED ${fmll_nn_perceptron_sources})
add_library(fmll_nn_som SHARED ${fmll_nn_som_sources})
add_library(fmll_nn_pca SHARED ${fmll_nn_pca_sources})
add_library(fmll_nn_svm SHARED ${fmll_nn_svm_sources})

target_link_libraries(fmll_lib ${PC_MXML_LIBRARIES})
target_link_libraries(fmll_math m fmll_lib)
target_link_libraries(fmll_nn_base fmll_lib fmll_math)
target_link_libraries(fmll_nn_perceptron fmll_nn_base)
target_link_libraries(fmll_nn_som fmll_nn_base)
target_link_libraries(fmll_nn_pca fmll_nn_base)
target_link_libraries(fmll_nn_svm fmll_nn_base)

if(Windows)

	set_target_properties(fmll_lib PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_math PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_nn_base PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_nn_perceptron PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_nn_som PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_nn_pca PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")
	set_target_properties(fmll_nn_svm PROPERTIES DEFINE_SYMBOL "COMPILING_DLL" SUFFIX ".dll")

endif()

install(TARGETS fmll_lib LIBRARY DESTINATION lib)
install(TARGETS fmll_math LIBRARY DESTINATION lib)
install(TARGETS fmll_nn_base LIBRARY DESTINATION lib)
install(TARGETS fmll_nn_perceptron LIBRARY DESTINATION lib)
install(TARGETS fmll_nn_som LIBRARY DESTINATION lib)
install(TARGETS fmll_nn_pca LIBRARY DESTINATION lib)
install(TARGETS fmll_nn_svm LIBRARY DESTINATION lib)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/fmll.h DESTINATION include/fmll)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/lib DESTINATION include/fmll FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/math DESTINATION include/fmll FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/nn DESTINATION include/fmll FILES_MATCHING PATTERN "*.h")

if(NOT Windows)

	configure_file(fmll.pc.in fmll.pc)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/fmll.pc DESTINATION lib/pkgconfig)

	add_subdirectory(examples examples)

endif()

