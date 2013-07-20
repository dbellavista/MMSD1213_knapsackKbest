/*
 * utility.c
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdlib.h>
#include "../include/kp_alg/utility.h"

void join_inner_solutions(InnerSolution** dest, InnerSolution* sol1,
		InnerSolution* sol2, uint16 count1, uint16 count2, uint16 K) {
	*dest = (InnerSolution*) malloc(
	K * sizeof(InnerSolution));
	uint16 i = 0, j = 0, k = 0;
	while (i < count1 && j < count2 && k < K) {
		InnerSolution val = sol1[i]->value > sol2[j]->value ? sol1[i++] : sol2[j++];
		(*dest)[k++] = val;
	}
	for (; i < count1 && k < K; i++) {
		(*dest)[k++] = sol1[i];
	}
	for (; j < count2 && k < K; j++) {
		(*dest)[k++] = sol2[j];
	}
	for(; k < K; k++) {
		(*dest)[k++] = NULL;
	}
}

void sort_by_values_non_inc(InnerSolution* sol, uint16 count) {
	byte done = 0;
	uint16 i, tmp;
	while (!done) {
		done = 1;
		for (i = 0; i < count - 1; i++) {
			if (sol[i]->value < sol[i + 1]->value) {
				done = 0;
				tmp = sol[i]->value;
				sol[i]->value = sol[i + 1]->value;
				sol[i + 1]->value = tmp;
			}
		}
	}

}

void sort_by_weights(KProblem problem) {

	byte done = 0;
	uint16 i, tmp;
	while (!done) {
		done = 1;
		for (i = 0; i < problem->n - 1; i++) {
			if (problem->weights[i] > problem->weights[i + 1]) {
				done = 0;
				tmp = problem->weights[i];
				problem->weights[i] = problem->weights[i + 1];
				problem->weights[i + 1] = tmp;
				tmp = problem->values[i];
				problem->values[i] = problem->values[i + 1];
				problem->values[i + 1] = tmp;
			}
		}
	}

}

void allocate_matrix(void*** matrix, uint32 width, uint32 height, uint32 size) {
	(*matrix) = (void**) malloc(width * sizeof(void*));
	void* vals = (void*) malloc(width * height * size);
	uint32 i;
	for (i = 0; i < width; i++) {
		(*matrix)[i] = &(vals[i * height * size]);
	}
}
void free_matrix(void** matrix) {
	free(matrix[0]);
	free(matrix);
}
