
#include <stdio.h>
#include "distance.h"
#include "som.h"

int main()
{
	unsigned u;
	uint8_t N[3] = {2, 3, 4};

	fmll_som * som = fmll_som_init(N, 3, 5, & fmll_som_weight_init_null, & fmll_distance_chebyshev, & fmll_distance_euclid);

	printf("%u\n", som->num);

	for(u = 0; u < som->num; u++)
		printf("%u = [%u, %u, %u]\n", u, som->coord[u][0], som->coord[u][1], som->coord[u][2]);

	fmll_som_destroy(som);

	return 0;
}

