/*
 * input.c
 *
 *  Created on: Aug 5, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/kp_alg/utility.h"
#include "../include/io_utility.h"
#include "../include/debug.h"

#define exit(X, Y) fclose(X); return Y;
#define check(X, Y, Z) if (X != Y) { d_error("Error reading from file\n"); exit(Z, false); }

void print_array(uint32* array, int size) {
	uint32 i;
	printf("[ ");
	for (i = 0; i < size; i++) {
		printf("%u ", array[i]);
	}
	printf("]\n");
}

void print_kproblem(KProblem problem) {
	uint32 size = problem->num_var;
	printf("Problem with %d variables, max weight: %d\n", problem->num_var, problem->max_weigth);
	printf("\tValues ");
	print_array(problem->values, size);
	printf("\tWeights ");
	print_array(problem->weights, size);
}

void print_kbest_solution(KBestSolutions solution) {
	uint32 size = solution->problem->num_var;
	printf("Solutions:\n");
	uint32 i;
	for (i = 0; i < solution->sol_count; i++) {
		printf("\t%d)\t%d\t-> ", i + 1, solution->solutions[i]->tot_value);
		print_array(solution->solutions[i]->solution_vector, size);
	}
	printf("\n");
}

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
	 * <V1> <W1>
	 * <V2> <W2>
	 * ...
	 * <Vn> <Wn>
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
		check(fscanf(fp, "%u %u", &values[i], &weights[i]), 2, fp);
		d_debug("V%d = %d; W%d = %d\n", (i + 1), values[i], (i + 1),
				weights[i]);
	}

	d_debug("Problem read\n\n");
	kp_init_kp(dest, N, weights, values, W);

	exit(fp, true);
}
