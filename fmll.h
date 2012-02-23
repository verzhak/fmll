
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

\en

	\mainpage My Personal Index Page

	TODO

	\section intro_sec Introduction

	This is the introduction.

	\section install_sec Installation

	\subsection step1 Step 1: Opening the box
 
	etc...

\ru

	\mainpage My Personal Index Page

	TODO

\endlang

*/

/* TODO убрать (C89 не поддерживает - заменить) */
#include <stdbool.h>

#ifdef FMLL_BUILD

	#include <lib/exception.h>
	#include <lib/memory.h>
	#include <lib/print.h>
	#include <math/constant.h>
	#include <math/matrix.h>
	#include <ann/base/distance.h>
	#include <ann/base/timing.h>
	#include <ann/base/various.h>
	#include <ann/base/weight_init.h>
	#include <ann/perceptron/activate.h>
	#include <ann/perceptron/perceptron.h>
	#include <ann/som/som.h>
	#include <ann/pca/pca.h>
	#include <ann/svm/kernel.h>
	#include <ann/svm/svm.h>
	#include <ann/svm/svm_net.h>

#else

	#include <fmll/lib/exception.h>
	#include <fmll/lib/memory.h>
	#include <fmll/lib/print.h>
	#include <fmll/math/constant.h>
	#include <fmll/math/matrix.h>
	#include <fmll/ann/base/distance.h>
	#include <fmll/ann/base/timing.h>
	#include <fmll/ann/base/various.h>
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

