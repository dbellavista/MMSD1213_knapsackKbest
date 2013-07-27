/*
 * utility.c
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdlib.h>
#include "../include/kp_alg/utility.h"

void sum_solution_vectors(InnerSolution dest, InnerSolution s1,
		InnerSolution s2) {
	uint32 i;
	for(i = 0; i < dest->dimension; i++) {
		dest->sol_vector[i] = s1->sol_vector[i] + s2->sol_vector[i];
	}
}

int find_idx_insertion(InnerSolution* sol_list, uint32 sols_size,
uint32 limit_idx, uint32 value) {
	int i = sols_size;

	if (limit_idx == 0 && sol_list[0]->value < value) {
		return 0;
	}
	if (sol_list[sols_size - 1]->value > value) {
		return -1;
	}

	for (i = sols_size - 1; i >= limit_idx; i--) {
		if (sol_list[i]->value >= value) {
			return i;
		}
	}
	return -1;
}

int find_idx(uint32* vector, uint32 start, uint32 fin, uint32 value) {
	uint32 i;
	for (i = start; i < fin; i++) {
		if (vector[i] == value) {
			return i;
		}
	}
	return -1;
}

void join_inner_solutions(InnerSolution* dest, InnerSolution* sol1,
		InnerSolution* sol2, uint32 count1, uint32 count2, uint32 K,
		bool free_garbage) {
	uint32 i = 0, j = 0, k = 0;
	while (i < count1 && j < count2 && k < K) {
		InnerSolution val =
				sol1[i]->value > sol2[j]->value ? sol1[i++] : sol2[j++];
		dest[k++] = val;
	}
	for (; i < count1; i++) {
		if (k < K) {
			dest[k++] = sol1[i];
		} else if (free_garbage) {
			kp_free_inn_sol(sol1[i]);
		}
	}
	for (; j < count2; j++) {
		if (k < K) {
			dest[k++] = sol2[j];
		} else if (free_garbage) {
			kp_free_inn_sol(sol2[j]);
		}
	}
	for (; k < K; k++) {
		dest[k++] = NULL;
	}
}

void sort_by_values_non_inc(InnerSolution* sol, uint32 count) {
	byte done = 0;
	uint32 i, tmp;
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
	uint32 i, tmp;
	while (!done) {
		done = 1;
		for (i = 0; i < problem->num_var - 1; i++) {
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

void allocate_matrix(void*** matrix, uint32 n_row, uint32 n_cols, uint32 size) {
	(*matrix) = (void**) malloc(n_row * sizeof(void*));
	void* vals = (void*) malloc(n_row * n_cols * size);
	uint32 i;
	for (i = 0; i < n_row; i++) {
		(*matrix)[i] = &(vals[i * n_cols * size]);
	}
}
void free_matrix(void** matrix) {
	free(matrix[0]);
	free(matrix);
}
