
/*!

\file

\en

	\brief TRANSLATE

\ru

	\brief Главный заголовочный файл библиотеки

\endlang

*/

#ifndef FMLL_H
#define FMLL_H

/*! 

\mainpage FMLL - Four of Meaningless Letters Library

\en

	TRANSLATE.

\ru

Нейросетевая (и не только) библиотека для языков программирования C / C++

\section system_req Системные требования

- компиляторы для языков программирования C и C++ из состава GNU Compiler Collection (для ОС GNU / Linux), MinGW (для кросскомпиляции для ОС семейства Windows) или Visual C++ из состава Visual Studio 2005 (для компиляции для ОС семейства Windows)
- утилиты автоматизации сборки CMake и GNU Make (в случае, если сборка осуществляется с помощью gcc или MinGW)
- система документации исходного кода Doxygen

\section build Сборка библиотеки

\subsection build_gnu_linux ОС GNU / Linux

\code
	
	cd build

	cmake .. - для сборки отладочной версии
	cmake -DCMAKE_BUILD_TYPE=Release .. - для сборки оптимизированной версии

	make
	make install
	
\endcode

\subsection build_windows_mingw ОС семейства Windows

(кросскомпиляция с помощью коллекции компиляторов MinGW)

\code
	
	cd build

	cmake -DWindows=1 .. - для сборки отладочной версии
	cmake -DWindows=1 -DCMAKE_BUILD_TYPE=Release .. - для сборки оптимизированной версии

	make
	
\endcode

или

(сборка с помощью IDE Visual Studio 2005)

- перейдите в каталог visual_studio
- переместите файлы fmll.* (пять штук) на уровень выше (в корневой каталог проекта)
- соберите проект с помощью Visual C++

\section build_doc_gen Генерирование документации

\code
	
	cd doc

	doxygen Doxyfile_en - для сборки документации на английском языке
	doxygen Doxyfile_ru - для сборки документации на русском языке
	
\endcode

\section Подключение

\subsection build_link_linux ОС GNU/Linux

\code

	#include <fmll/fmll.h>

	pkg-config --cflags fmll
	pkg-config --libs fmll
	
\endcode

\subsection build_link_windows ОС Windows

- подключите библиотеку
- \code #include <fmll/fmll.h> \endcode
- заголовочные файлы библиотеки рассортированы по подкаталогам корневого каталога проекта - рекомендуется указывать путь к корневому каталогу проекта в свойствах производного проекта
- подключайте к программе библиотеку mini-xml (каталог mxml; библиотека mini-xml собрана как динамическая библиотека из-за опасения нарушить лицензионное соглашение в случае сборки ее статической версии)

\section author Автор и способы связи

Автор: Акинин М.В.

E-mail: verzhak@gmail.com

Дата: 17.05.2013

\endlang

*/

#ifndef __cplusplus

	#include <stdbool.h>

#endif

#ifdef FMLL_BUILD

	#include "lib/exception.h"
	#include "lib/memory.h"
	#include "lib/print.h"
	#include "math/graph/graph.h"
	#include "math/matrix/matrix.h"
	#include "math/random/random.h"
	#include "math/various/constant.h"
	#include "math/various/distance.h"
	#include "math/various/various.h"
	#include "ann/base/activate.h"
	#include "ann/base/timing.h"
	#include "ann/base/weight_init.h"
	#include "ann/ff/ff.h"
	#include "ann/mlp/mlp.h"
	#include "ann/som/som.h"
	#include "ann/pca/pca.h"
	#include "ann/svm/kernel.h"
	#include "ann/svm/svm.h"
	#include "ann/svm/svm_net.h"

#else

	#include <fmll/lib/exception.h>
	#include <fmll/lib/memory.h>
	#include <fmll/lib/print.h>
	#include <fmll/math/graph/graph.h>
	#include <fmll/math/matrix/matrix.h>
	#include <fmll/math/random/random.h>
	#include <fmll/math/various/constant.h>
	#include <fmll/math/various/distance.h>
	#include <fmll/math/various/various.h>
	#include <fmll/ann/base/activate.h>
	#include <fmll/ann/base/timing.h>
	#include <fmll/ann/base/weight_init.h>
	#include <fmll/ann/ff/ff.h>
	#include <fmll/ann/mlp/mlp.h>
	#include <fmll/ann/som/som.h>
	#include <fmll/ann/pca/pca.h>
	#include <fmll/ann/svm/kernel.h>
	#include <fmll/ann/svm/svm.h>
	#include <fmll/ann/svm/svm_net.h>

#endif

#endif

