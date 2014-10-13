#include "laplace.h"

long int turn;

void laplace_init()
{
	turn=0;
	field_init();

	return;
}

void laplace()
{
	uint8_t field_changed;

#pragma omp parallel
{
	int i, j;
	int fc, fn;
	int tid, nthreads;

	tid=omp_get_thread_num();
	nthreads=omp_get_num_threads();

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

#pragma omp single
		{
			turn++;
		}
	}
}

	return;
}
