#include "laplace.h"

void laplace_init()
{
	field_init();

	return;
}

void laplace()
{
	uint8_t field_changed;
	long int turn=0;

#pragma omp parallel firstprivate(turn)
{
	int i, j;
	int fc, fn;
	int tid, nthreads;

	turn=0;
	tid=omp_get_thread_num();
	nthreads=omp_get_num_threads();

#pragma omp single
{
	printf("nthreads: %d\n", nthreads);
}

	for(;;){
#pragma omp single
		{
			printf("turn: %ld\n", turn);
			field_output_null(turn);
			field_changed=0;
		}

		fc=turn%2;
		fn=(fc+1)%2;
#pragma omp for collapse(2) private(i,j) reduction(|:field_changed)
		for(i=1; i<1+M; i++){
			for(j=1; j<1+N; j++){
				field[fn][i][j]=(field[fc][i-1][j]+field[fc][i][j-1]+field[fc][i+1][j]+field[fc][i][j+1])/4;
				if((!field_changed) && field[fc][i][j]!=field[fn][i][j])
					field_changed=1;
			}
		}
		if(!field_changed)
			break;

		turn++;
#pragma omp barrier
	}
}

	return;
}
