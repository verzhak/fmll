
/*!

\file

\brief Функции инициализации весов нейронов нейронных сетей.

*/

#ifndef FMLL_WEIGHT_INIT_H
#define FMLL_WEIGHT_INIT_H

#ifdef FMLL_BUILD

#include "all.h"

#endif

/*!

\brief Инициализация весов нейронов нейронной сети нулями.

\return ноль.

*/
double fmll_weight_init_null();

/*!

\brief Инициализация весов нейронов нейронной сети значением 0.5.

\return ноль.

*/
double fmll_weight_init_0_5();

/*!

\brief Инициализация весов нейронов нейронной сети случайными равномернораспределенными значениями из диапазона [0, 1).

Случайное число генерируется с помощью функции drand48(). Датчик случайных чисел должен быть предварительно проинициализирован вызовом функции srand48().

\return случайное число из диапазона [0, 1).

*/
double fmll_weight_init_random_0_1();

/*!

\brief Инициализация весов нейронов нейронной сети случайными равномернораспределенными значениями из диапазона [0, 0.1).

Случайное число генерируется с помощью функции drand48(). Датчик случайных чисел должен быть предварительно проинициализирован вызовом функции srand48().

\return случайное число из диапазона [0, 0.1).

*/
double fmll_weight_init_random_0_01();

/*!

\brief Инициализация весов нейронов нейронной сети случайными равномернораспределенными значениями из диапазона [0, 0.01).

Случайное число генерируется с помощью функции drand48(). Датчик случайных чисел должен быть предварительно проинициализирован вызовом функции srand48().

\return случайное число из диапазона [0, 0.01).

*/
double fmll_weight_init_random_0_001();

/*!

\brief Инициализация весов нейронов нейронной сети случайными равномернораспределенными значениями из диапазона [-0.1, 0.1).

Случайное число генерируется с помощью функции drand48(). Датчик случайных чисел должен быть предварительно проинициализирован вызовом функции srand48().

\return случайное число из диапазона [-0.1, 0.1).

*/
double fmll_weight_init_random_m05_05();

#endif

