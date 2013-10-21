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
#include "kbest_io_utility.h"
#include "utility/debug.h"
#include "utility/utility.h"

#define exit(X, Y) fclose(X); return Y;
#define check(X, Y, Z) if (X != Y) { d_error("Error reading from file\n"); exit(Z, false); }

void print_array(uint32_t* array, size_t size);

void print_array(uint32_t* array, size_t size) {
	uint32_t i;
	printf("[ ");
	for (i = 0; i < size; i++) {
		printf("%u ", array[i]);
	}
	printf("]\n");
}

void print_kproblem(KProblem problem) {
	uint32_t size = problem->num_var;
	printf("Problem with %zu variables, max weight: %d\n", problem->num_var, problem->max_weigth);
	printf("\tValues ");
	print_array(problem->values, size);
	printf("\tWeights ");
	print_array(problem->weights, size);
}

void print_kbest_solution(KBestSolutions solution) {
	uint32_t size = solution->problem->num_var;
	printf("Solutions:\n");
	size_t i;
	for (i = 0; i < solution->sol_count; i++) {

		printf("\t%zu)\t%u\t-> ", i + 1, solution->solutions[i]->tot_value);
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
	uint32_t N, W, i;
	uint32_t *weights, *values;

	check(fscanf(fp, "%u", &N), 1, fp);
	d_debug("Read N = %d\n", N);

	check(fscanf(fp, "%u", &W), 1, fp);
	d_debug("Read W = %d\n", W);

	weights = (uint32_t*) malloc(N * sizeof(uint32_t));
	values = (uint32_t*) malloc(N * sizeof(uint32_t));

	for (i = 0; i < N; i++) {
		check(fscanf(fp, "%u %u", &values[i], &weights[i]), 2, fp);
		d_debug("Read var V%d = %d; W%d = %d\n", (i + 1), values[i], (i + 1),
				weights[i]);
	}

	d_debug("Problem read\n\n");
	kp_init_kp(dest, N, weights, values, W);

	exit(fp, true);
}
