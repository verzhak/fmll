
/*!

\file

\en

	\brief TODO

\ru

	\brief Главный заголовочный файл библиотеки

\endlang

*/

#ifndef FMLL_H
#define FMLL_H

/*! 

\mainpage FMLL - Four of Meaningless Letters Library

\en

	TODO.

\ru

	Нейросетевая (и не только) библиотека для языков программирования C / C++

	\section system_req Системные требования

	-# Компиляторы для языков программирования C и C++ из состава GNU Compiler Collection (для ОС GNU / Linux) или MinGW (для кросскомпиляции для ОС семейства Windows)
	-# Утилиты автоматизации сборки CMake и GNU Make
	-# Система документации исходного кода Doxygen

	\section build Сборка библиотеки

		\subsection build_gnu_linux ОС GNU / Linux

		-# cd build
		-#
			- cmake .. - для сборки отладочной версии
			- cmake -DCMAKE_BUILD_TYPE=Release .. - для сборки оптимизированной версии
		-# make
		-# make install

		\subsection build_windows ОС семейства Windows

		(кросскомпиляция с помощью коллекции компиляторов MinGW)

		-# cd build
		-#
			- cmake -DWindows=1 .. - для сборки отладочной версии
			- cmake -DWindows=1 -DCMAKE_BUILD_TYPE=Release .. - для сборки оптимизированной версии
		-# make

	\section build_doc_gen Генерирование документации

		-# cd doc
		-#
			- doxygen Doxyfile_en - для сборки англоязычной документации
			- doxygen Doxyfile_ru - для сборки русскоязычной документации

	\section build_link Подключение

		-# #include <fmll/fmll.h>
		-# pkg-config --cflags fmll
		-# pkg-config --libs fmll

	\section Автор и способы связи

	Автор: Акинин М.В. (магистрант Рязанского Государственного Радиотехнического Университета, кафедра ЭВМ)

	E-mail: verzhak@gmail.com

	Дата: 26.02.2012

\endlang

*/

#ifndef __cplusplus

	#include <stdbool.h>

#endif

#ifdef FMLL_BUILD

	#include "lib/exception.h"
	#include "lib/memory.h"
	#include "lib/print.h"
	#include "math/matrix/matrix.h"
	#include "math/random/random.h"
	#include "math/various/constant.h"
	#include "math/various/distance.h"
	#include "math/various/various.h"
	#include "ann/base/timing.h"
	#include "ann/base/weight_init.h"
	#include "ann/perceptron/activate.h"
	#include "ann/perceptron/perceptron.h"
	#include "ann/som/som.h"
	#include "ann/pca/pca.h"
	#include "ann/svm/kernel.h"
	#include "ann/svm/svm.h"
	#include "ann/svm/svm_net.h"

#else

	#include <fmll/lib/exception.h>
	#include <fmll/lib/memory.h>
	#include <fmll/lib/print.h>
	#include <fmll/math/matrix/matrix.h>
	#include <fmll/math/random/random.h>
	#include <fmll/math/various/constant.h>
	#include <fmll/math/various/distance.h>
	#include <fmll/math/various/various.h>
	#include <fmll/ann/base/timing.h>
	#include <fmll/ann/base/weight_init.h>
	#include <fmll/ann/perceptron/activate.h>
	#include <fmll/ann/perceptron/perceptron.h>
	#include <fmll/ann/som/som.h>
	#include <fmll/ann/pca/pca.h>
	#include <fmll/ann/svm/kernel.h>
	#include <fmll/ann/svm/svm.h>
	#include <fmll/ann/svm/svm_net.h>

#endif

#endif

