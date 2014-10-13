#include "main.h"
#include <sys/time.h>

int main()
{
	struct timeval start, end;

	laplace_init();

	/*field_random(turn);*/
	field_square(turn);
	/*field_circle(turn);*/


	gettimeofday(&start, NULL);

	laplace();

	gettimeofday(&end, NULL);

	printf("time: %g\n", (end.tv_sec+end.tv_usec*1e-6)-(start.tv_sec+start.tv_usec*1e-6));

	return 0;
}
