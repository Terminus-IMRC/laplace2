#include "laplace.h"

int fc, fn;
uint8_t *field_changed_g;
pthread_barrier_t allbrr;
int nthreads;

void laplace_init()
{
	int e;
	char *s;

	s=getenv("LAPLACE_NUM_THREADS");
	if(s!=NULL){
		nthreads=atoi(s);
		if(nthreads<=0)
			nthreads=ncpus();
	}else
		nthreads=ncpus();

	printf("nthreads: %d\n", nthreads);

	field_init();

	e=pthread_barrier_init(&allbrr, NULL, nthreads);
	if(e!=0){
		errno=e;
		perror("pthread_barrier_init");
		exit(EXIT_FAILURE);
	}

	field_changed_g=(uint8_t*)calloc(nthreads, sizeof(uint8_t));
	assert(field_changed_g);

	return;
}

void* laplace(void *arg)
{
	long int turn=0;
	int i, j;
	uint8_t field_changed;
	int tid=(int)arg;
	int M_start, M_end;

	M_start=1+(M*tid/nthreads);
	M_end=1+(M*(tid+1)/nthreads);

	for(;;){
		if(tid==0){
			printf("turn: %ld\n", turn);
			field_output_null(turn);
		}

		fc=turn%2;
		fn=(fc+1)%2;
		field_changed=0;
		for(i=M_start; i<M_end; i++){
			for(j=1; j<1+N; j++){
				field[fn][i][j]=(field[fc][i-1][j]+field[fc][i][j-1]+field[fc][i+1][j]+field[fc][i][j+1])/4;
				if((!field_changed) && field[fc][i][j]!=field[fn][i][j])
					field_changed=1;
			}
		}
		field_changed_g[tid]=field_changed;

		pthread_barrier_wait(&allbrr);
		if(tid==0){
			for(i=0; i<nthreads; i++){
				if(field_changed_g[i]==1){
					field_changed_g[0]=1;
					break;
				}
			}
		}

		turn++;

		pthread_barrier_wait(&allbrr);

		if(field_changed_g[0]==0)
			break;

		pthread_barrier_wait(&allbrr);
	}

	return NULL;
}
