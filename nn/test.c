
#include "all.h"
#include "test.h"

/*! \brief Тестовая функция
 *
 * \param x - целое;
 * \param y - целое.
 *
 * \return
 *
 *	- 0 - в случае успеха;
 *	- <> 0 - в случае неудачи.
 *
 * \sa
 *
 *  - main().
 */

int test(int x, int y)
{
#ifdef DEBUG

	printf("TODO IN test DEBUG %d\n", x + y);

#else

	printf("TODO IN test RELEASE %d\n", x + y);

#endif

	return 0;
}

