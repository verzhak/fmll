
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

#ifndef __cplusplus

	// C

	#include <stdbool.h>

#endif

#ifdef FMLL_BUILD

	#include <lib/exception.h>
	#include <lib/memory.h>
	#include <math/matrix.h>
	#include <nn/activate.h>
	#include <nn/distance.h>
	#include <nn/timing.h>
	#include <nn/various.h>
	#include <nn/weight_init.h>
	#include <nn/kernel.h>
	#include <nn/perceptron.h>
	#include <nn/som.h>
	#include <nn/pca.h>
	#include <nn/svm.h>
	#include <nn/svm_net.h>

#else

	#include <fmll/lib/exception.h>
	#include <fmll/lib/memory.h>
	#include <fmll/math/matrix.h>
	#include <fmll/nn/activate.h>
	#include <fmll/nn/distance.h>
	#include <fmll/nn/timing.h>
	#include <fmll/nn/various.h>
	#include <fmll/nn/weight_init.h>
	#include <fmll/nn/kernel.h>
	#include <fmll/nn/perceptron.h>
	#include <fmll/nn/som.h>
	#include <fmll/nn/pca.h>
	#include <fmll/nn/svm.h>
	#include <fmll/nn/svm_net.h>

#endif

#endif

