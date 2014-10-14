#include "laplace.h"

long int turn;
int fc, fn;
uint8_t *field_changed_g;
pthread_barrier_t **brr, allbrr;
int nthreads;

void laplace_init()
{
	int i, e;
	pthread_barrier_t *brr_1dim;
	char *s;

	s=getenv("LAPLACE_NUM_THREADS");
	if(s!=NULL){
		nthreads=atoi(s);
		if(nthreads<=0)
			nthreads=ncpus();
	}else
		nthreads=ncpus();

	printf("nthreads: %d\n", nthreads);

	turn=0;
	field_init();

	e=pthread_barrier_init(&allbrr, NULL, nthreads);
	if(e!=0){
		errno=e;
		perror("pthread_barrier_init");
		exit(EXIT_FAILURE);
	}

	brr_1dim=(pthread_barrier_t*)malloc((floor(nthreads/2.0)*nthreads)*sizeof(pthread_barrier_t));
	assert(brr_1dim);

	brr=(pthread_barrier_t**)malloc((floor(nthreads/2.0))*sizeof(pthread_barrier_t*));
	assert(brr);

	field_changed_g=(uint8_t*)calloc(nthreads, sizeof(uint8_t));
	assert(field_changed_g);

	for(i=0; i<floor(nthreads/2.0)*nthreads; i++){
		e=pthread_barrier_init(brr_1dim+i, NULL, 2);
		if(e!=0){
			errno=e;
			perror("pthread_barrier_t");
			exit(EXIT_FAILURE);
		}
	}

	for(i=0; i<nthreads/2; i++){
		brr[i]=brr_1dim+(nthreads*i);
	}

	return;
}

void* laplace(void *arg)
{
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

		if(turn%TURN_GATHER_INTERVAL==0){
			field_changed_g[tid]=field_changed;

			for(i=0; i<(int)ceil(log(nthreads)/log(2)); i++){
				if(tid%(int)pow(2, i)!=0)
					break;
				if((int)(tid/pow(2, i))%2==0){
					if(tid+pow(2, i)>=nthreads)
						continue;
					pthread_barrier_wait(&(brr[tid/2][(int)(tid+pow(2, i))]));
					field_changed_g[tid]=(field_changed_g[tid]!=0 || field_changed_g[(int)(tid+pow(2, i))]!=0)?1:0;
				}else{
					pthread_barrier_wait(&(brr[(int)((tid-pow(2, i))/2)][tid]));
				}
			}

			pthread_barrier_wait(&allbrr);

			if(field_changed_g[0]==0)
				break;
		}else
			pthread_barrier_wait(&allbrr);

		if(tid==0)
			turn++;

		pthread_barrier_wait(&allbrr);
	}

	return NULL;
}
