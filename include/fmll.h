
/*!

\file

\brief Главный заголовочный файл библиотеки.

*/

#ifndef FMLL_H
#define FMLL_H

/*! \mainpage My Personal Index Page
 * 
 * TODO
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *  
 * etc...
 */

#include <stdbool.h>

#ifdef FMLL_BUILD

#include <activate.h>
#include <distance.h>
#include <exception.h>
#include <memory.h>
#include <perceptron.h>
#include <som.h>
#include <timing.h>
#include <various.h>
#include <weight_init.h>
#include <pca.h>
#include <kernel.h>
#include <svm.h>
#include <svm_net.h>
#include <mathematic.h>

#else

#include <fmll/activate.h>
#include <fmll/distance.h>
#include <fmll/exception.h>
#include <fmll/memory.h>
#include <fmll/perceptron.h>
#include <fmll/som.h>
#include <fmll/timing.h>
#include <fmll/various.h>
#include <fmll/weight_init.h>
#include <fmll/pca.h>
#include <fmll/kernel.h>
#include <fmll/svm.h>
#include <fmll/svm_net.h>
#include <fmll/mathematic.h>

#endif

#endif

