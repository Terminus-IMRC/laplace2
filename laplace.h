#ifndef __LAPLACE_H_INSIDE__
#define __LAPLACE_H_INSIDE__

#define __USE_XOPEN2K
#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stddef.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include "field.h"
#include "ncpus.h"

	extern long int turn;
	extern int fc, fn;
	extern uint8_t *field_changed_g;
	extern pthread_barrier_t **brr;
	extern pthread_barrier_t allbrr;
	extern int nthreads;

	void laplace_init();
	void* laplace(void *arg);

#endif /* __LAPLACE_H_INSIDE__ */
