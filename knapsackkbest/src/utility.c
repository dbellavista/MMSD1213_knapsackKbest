/*
 * utility.c
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdlib.h>
#include <string.h>
#include "utility.h"

bool inner_solutions_equals(InnerSolution sol1, InnerSolution sol2)
{
  if(sol1->last_zero != sol2->last_zero) {
    return false;
  }
  if(memcmp(sol1->bitmap_sol_vector, sol2->bitmap_sol_vector, BITMAP_DYNAMIC_BYTE_SIZE(sol1))) {
    return false;
  }
  if(memcmp(sol1->sol_vector, sol2->sol_vector, sol1->last_zero * sizeof(*sol1))) {
    return false;
  }
  return true;
}

void create_kbest_solutions_from_inner(KBestSolutions* bestSolutions,
		InnerSolution* solutions, size_t solutions_size, KProblem problem,
		bool free_inner)
{
	uint32_t i, k;
	kp_init_kbest_sols(bestSolutions, problem, solutions_size);
	for (i = 0; i < solutions_size; i++) {
		InnerSolution is = solutions[i];
		KSolution sol;
		kp_init_sol(&sol, problem->num_var);

		sol->tot_value = is->value;
		for (k = 0; k < problem->num_var; k++) {
			sol->solution_vector[k] = is->sol_vector[k];
		}
		(*bestSolutions)->solutions[i] = sol;
	}

	if (free_inner) {
		for (i = 0; i < solutions_size; i++) {
			kp_free_inn_sol(solutions[i]);
		}
		free(solutions);
	}
}

void sum_solution_vectors(InnerSolution dest, InnerSolution s1,
		InnerSolution s2)
{
	uint32_t i;
	for (i = 0; i < dest->dimension; i++) {
		dest->sol_vector[i] = s1->sol_vector[i] + s2->sol_vector[i];
	}
}

ssize_t find_idx_insertion(InnerSolution* sol_list, size_t sols_size, size_t lower_limit_idx, uint32_t value)
{
	uint32_t middle;
	uint32_t lower = lower_limit_idx + 1;
	uint32_t upper = sols_size - 1;

	if(sol_list[lower_limit_idx]->value < value) {
		return 0;
	}

	while (true) {
		middle = (lower + upper) / 2;
		if (sol_list[middle]->value >= value) {
			if (sol_list[middle + 1]->value < value) {
				return middle + 1;
			} else if (lower != middle) {
				lower = middle;
			} else {
				return -1;
			}
		} else if (sol_list[middle]->value < value) {
			if (sol_list[middle - 1]->value >= value) {
				return middle;
			} else if (middle != upper) {
				upper = middle;
			} else {
				return -1;
			}
		}
	}
}

ssize_t find_idx_and_prepare_insertion(InnerSolution* sol_list, size_t*
    sols_size, InnerSolution* removed, size_t lower_limit_idx, uint32_t value, size_t K)
{
	uint32_t i;
	int idx = find_idx_insertion(sol_list, *sols_size, lower_limit_idx, value);

	if (idx == -1) {
		return -1;
	}

	if (*sols_size == K) {
	  if(removed == NULL) {
      kp_free_inn_sol(sol_list[K - 1]);
    } else {
      *removed = sol_list[K - 1];
    }
	} else {
		*sols_size = *sols_size + 1;
	}
	for (i = *sols_size - 1; i > idx; i--) {
		sol_list[i] = sol_list[i - 1];
	}
	sol_list[idx] = NULL;

	return idx;
}

ssize_t find_idx(int* vector, size_t fin, int value)
{
	uint32_t i;
	for (i = 0; i <= fin; i++) {
		if (vector[i] == value) {
			return i;
		}
	}
	return -1;
}

void join_inner_solutions(InnerSolution* dest, InnerSolution* sol1,
		InnerSolution* sol2, size_t count1, size_t count2, size_t K,
		bool free_garbage)
{
	uint32_t i = 0, j = 0, k = 0;
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

void sort_by_values_non_inc(InnerSolution* sol, uint32_t count)
{
	byte done = 0;
	uint32_t i;
	InnerSolution tmp;
	while (!done) {
		done = 1;
		for (i = 1; i < count; i++) {
			if (sol[i - 1]->value < sol[i]->value) {
				done = 0;
				tmp = sol[i - 1];
				sol[i - 1] = sol[i];
				sol[i] = tmp;
			}
		}
	}
}

void sort_by_weights(KProblem problem)
{

	byte done = 0;
	uint32_t i, tmp;
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

void allocate_matrix(void*** matrix, uint32_t n_row, uint32_t n_cols, uint32_t size)
{
	(*matrix) = (void**) malloc(n_row * sizeof(void*));
	void* vals = (void*) malloc(n_row * n_cols * size);
	uint32_t i;
	for (i = 0; i < n_row; i++) {
		(*(char***)matrix)[i] = &(((char*)vals)[i * n_cols * size]);
	}
}

void free_matrix(void** matrix)
{
	free(matrix[0]);
	free(matrix);
}
