/**
 *
 * @file    kp_inner_model.c
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    10/21/2013 12:32:05 PM
 * @brief   Contains the model structure and utility function definition for
 *          the model used by the internal algorithms.
 *
 */

#include <stdlib.h>
#include <string.h>
#include "kp_inner_model.h"
#include "utility/utility.h"

void kp_init_inn_sol(InnerSolution* iSol, size_t n, size_t j, size_t t,
    uint32_t v)
{
	*iSol = (InnerSolution) malloc(sizeof(struct innerSolution));
	(*iSol)->dimension = n;
	(*iSol)->column_idx = j;
	(*iSol)->row_idx = t;
	(*iSol)->value = v;
	(*iSol)->recovered = false;
	// Sets the value to 0
	(*iSol)->sol_vector = (uint32_t*) calloc(n, sizeof(uint32_t));
}

void kp_free_inn_sol(InnerSolution innerSol)
{
	free(innerSol->sol_vector);
	innerSol->sol_vector = NULL;
	free(innerSol);
}

void kp_copy_inn_sol(InnerSolution* dest, InnerSolution origin)
{
	uint32_t i;
	kp_init_inn_sol(dest, origin->dimension, origin->column_idx,
			origin->row_idx, origin->value);
	(*dest)->recovered = origin->recovered;
	for (i = 0; i < origin->dimension; i++) {
		(*dest)->sol_vector[i] = origin->sol_vector[i];
	}
}

void create_kbest_solutions_from_inner(KBestSolutions* bestSolutions,
		InnerSolution* solutions, size_t solutions_size, KProblem problem,
		bool free_inner)
{
	uint32_t i, k;
	KSolution sol;
  InnerSolution is;

	kp_init_kbest_sols(bestSolutions, problem, solutions_size);

	for (i = 0; i < solutions_size; i++) {
		is = solutions[i];
		sol = (*bestSolutions)->solutions[i];
		sol->tot_value = is->value;
		for (k = 0; k < problem->num_var; k++) {
			sol->solution_vector[k] = is->sol_vector[k];
		}
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
	size_t i;
	for (i = 0; i < dest->dimension; i++) {
		dest->sol_vector[i] = s1->sol_vector[i] + s2->sol_vector[i];
	}
}

ssize_t find_idx_insertion(InnerSolution* sol_list, size_t sols_size, size_t
    size_max, ssize_t lower_limit_idx, uint32_t value)
{
	size_t middle, lower, upper;
	bool exit;

	if(sols_size == 0) {
    return (size_max > 0) ? 0 : -1;
  }

  if(lower_limit_idx >= 0 && sol_list[lower_limit_idx]->value < value) {
		return -1;
	}

	lower = lower_limit_idx + 1;
	upper = sols_size - 1;

  exit = false;
	while (!exit) {
		exit = lower == upper;
		middle = (lower + upper) / 2;
		if (sol_list[middle]->value >= value) {
			if (middle + 1 == sols_size) {
        return (sols_size < size_max) ? middle + 1 : -1;
      } else if(sol_list[middle + 1]->value < value) {
				return middle + 1;
			} else {
				lower = middle + 1;
			}
		} else if (sol_list[middle]->value < value) {
			if (middle == 0 || sol_list[middle - 1]->value >= value) {
				return middle;
			} else {
				upper = middle - 1;
			}
		}
	}
	return -1;
}

ssize_t find_idx_and_prepare_insertion(InnerSolution* sol_list, size_t*
    sols_size, ssize_t lower_limit_idx, uint32_t value, size_t K)
{
	ssize_t idx, i;
	idx = find_idx_insertion(sol_list, *sols_size, K, lower_limit_idx, value);

	if (idx == -1) {
		return idx;
	}

	if (*sols_size == K) {
    kp_free_inn_sol(sol_list[K - 1]);
    sol_list[K - 1] = NULL;
	} else {
		*sols_size = *sols_size + 1;
	}
	for (i = *sols_size - 1; i > idx; i--) {
		sol_list[i] = sol_list[i - 1];
	}
	sol_list[idx] = NULL;

	return idx;
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

void print_inner_solution(InnerSolution sol)
{
  size_t i;
  printf("Solution v:%u, row:%zu, col:%zu, rec:%d\n", sol->value, sol->row_idx,
      sol->column_idx, sol->recovered);

  printf("\t[");
  for(i = 0; i < sol->dimension; i++) {
    printf("%3u", sol->sol_vector[i]);
  }
  printf("]\n");
}
