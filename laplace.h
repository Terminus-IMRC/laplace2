#ifndef __LAPLACE_H_INSIDE__
#define __LAPLACE_H_INSIDE__

#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include "field.h"
#include "ncpus.h"

	extern long int turn;
	extern int fc, fn;

	void laplace_init();
	void laplace();

#endif /* __LAPLACE_H_INSIDE__ */
