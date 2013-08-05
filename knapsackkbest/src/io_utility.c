/*
 * input.c
 *
 *  Created on: Aug 5, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/kp_alg/utility.h"
#include "../include/io_utility.h"
#include "../include/debug.h"

#define exit(X, Y) fclose(X); return Y;
#define check(X, Y, Z) if (X != Y) { d_error("Error reading from file\n"); exit(Z, false); }

bool read_problem(KProblem* dest, char* file) {

	*dest = NULL;
	FILE* fp = fopen(file, "r");
	if (fp == NULL) {
		d_error("Error opening file %s: %s\n", file, strerror(errno));
		return false;
	}

	/*
	 * Format:
	 * <Num Var>
	 * <Max W>
	 * <W1> <V1>
	 * <W2> <V2>
	 * ...
	 * <Wn> <Vn>
	 */
	uint32 N, W, i;
	uint32 *weights, *values;

	d_debug("Reading N... ");
	check(fscanf(fp, "%u", &N), 1, fp);
	d_debug("N = %d\n", N);

	d_debug("Reading W... ");
	check(fscanf(fp, "%u", &W), 1, fp);
	d_debug("W = %d\n", W);

	weights = (uint32*) malloc(N * sizeof(uint32));
	values = (uint32*) malloc(N * sizeof(uint32));

	for (i = 0; i < N; i++) {
		d_debug("Reading var %u... ", (i + 1));
		check(fscanf(fp, "%u %u", &weights[i], &values[i]), 2, fp);
		d_debug("W%d = %d; V%d = %d\n", (i + 1), weights[i], (i + 1),
				values[i]);
	}

	kp_init_kp(dest, N, weights, values, W);

	exit(fp, true);
}
