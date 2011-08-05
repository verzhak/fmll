
/*!

\file

\brief Нейронная сеть, используемая для анализа главных компонент (PCA - Principal Component Analysis), обучаемая по принципу самоорганизации корреляционного типа.

Последовательность действий по использованию PCA-сети:

	-# Создать PCA-сеть с помощью функции fmll_pca_init();
	-# Обучить (самоорганизация) PCA-сеть с помощью одной из *_pca_so_* функций;
	-# Прогнать PCA-сеть над целевыми векторами с помощью функции fmll_pca_run();
	-# Удалить PCA-сеть с помощью функции fmll_pca_destroy().

*/

#ifndef PCA_H
#define PCA_H

#include "all.h"
#include "exception.h"
#include "memory.h"
#include "xml.h"
#include "weight_init.h"

// ############################################################################ 

/*!

\brief Описатель PCA-сети.

*/
typedef struct t_fmll_pca
{

	/*! Весы синапсов нейронов. */
	double ** w;

	/*! Вектор выходных значений. */
	double * y;

	/*! Размерность исходного векторного пространства. */
	uint8_t dim;

	/*!
	 * Количество нейронов (размерность целевого векторного пространства).
	 * Должно выполняться условие: \f$ 0 ~ < ~ num ~ \le ~ dim \f$.
	 */
	uint8_t num;

} fmll_pca;

// ############################################################################ 

/*!

\brief Создание PCA-сети.

\param dim - размерность исходного векторного пространства;
\param num - количество нейронов в PCA-сети (размерность целевого векторного пространства);
\param weight_init - указатель на функцию, инициализирующую веса синапсов нейронов.

Должно выполняться условие: \f$ 0 ~ < ~ num ~ \le ~ dim \f$.

\return

	- указатель на описатель PCA-сети в случае ее успешного создания;
	- NULL - в случае неудачи.

*/
fmll_pca * fmll_pca_init(uint8_t dim, uint8_t num, double (* weight_init)());

/*!

\brief Удаление PCA-сети.

\param pca - указатель на описатель PCA-сети.

*/
void fmll_pca_destroy(fmll_pca * pca);

/*!

\brief Сохранение в XML-файл описателя PCA-сети.

\param pca - указатель на описатель PCA-сети;
\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

\return

	- 0 - в случае успешного сохранения описателя PCA-сети;
	- <> 0 - в случае некорректного завершения операции сохранения описателя PCA-сети.

*/
int8_t fmll_pca_save(fmll_pca * pca, const char * fname_prefix);

/*!

\brief Загрузка из XML-файла описателя PCA-сети.

\param fname_prefix - путь и имя XML-файла (к строке fname_prefix будет добавлено расширение .xml).

\return

	- указатель на описатель PCA-сети в случае его успешной загрузки;
	- NULL - в случае некорректного завершения операции загрузки описателя PCA-сети.

*/
fmll_pca * fmll_pca_load(const char * fname_prefix);

/*!

\brief Прогон PCA-сети над некоторым вектором.

\param pca - указатель на описатель PCA-сети;
\param vec - некоторый вектор.

\return указатель на результирующий вектор.

*/
const double * fmll_pca_run(fmll_pca * pca, const double * vec);

/*!

\brief Обучение (самоорганизация) PCA-сети по корреляционному принципу.

\param PCA - дескриптор PCA-сети;
\param vec - массив векторов;
\param vec_num - количество векторов в массиве векторов;
\param max_d - максимальное изменение весов нейронов PCA-сети на очередного итерации алгоритма самоорганизации, при котором процесс самоорганизации будет остановлен.

\return

	- 0 - в случае успеха;
	- <> 0 - в случае неудачи.

*/
int8_t fmll_pca_so(fmll_pca * pca, double ** vec, uint32_t vec_num, double max_d);

// ############################################################################ 

#endif
