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

	(*iSol)->last_zero = 0;
	(*iSol)->bitmap_sol_vector = (bitmap_t*) calloc(BITMAP_DYNAMIC_BYTE_SIZE(*iSol), sizeof(char));
}

void kp_free_inn_sol(InnerSolution innerSol)
{
	free(innerSol->sol_vector);
	innerSol->sol_vector = NULL;
	free(innerSol->bitmap_sol_vector);
	innerSol->bitmap_sol_vector = NULL;
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
	(*dest)->last_zero = origin->last_zero;
  memcpy((*dest)->bitmap_sol_vector, origin->bitmap_sol_vector, BITMAP_DYNAMIC_BYTE_SIZE(origin));
}

void set_inner_sol_element(InnerSolution isol, size_t var, uint32_t value)
{
  size_t index;
  bitmap_t mask;

  isol->sol_vector[var] = value;
  index = var / (8 * sizeof(bitmap_t));
  mask = 1 << (var % (8 * sizeof(bitmap_t)));
  if(value) {
    isol->bitmap_sol_vector[index] |= mask;
  } else {
    isol->bitmap_sol_vector[index] &= ~mask;
  }

  if (value && var >= isol->last_zero) {
    isol->last_zero = var + 1;
  } else if(value == 0 && var == isol->last_zero - 1) {
    isol->last_zero = var;
  }
}

bool inner_solutions_equal(InnerSolution sol1, InnerSolution sol2)
{
  if(!sol1->recovered || !sol2->recovered) {
    return false;
  }
  if(sol1->value != sol2->value) {
    return false;
  }
  if(sol1->last_zero != sol2->last_zero) {
    return false;
  }
  if(memcmp(sol1->bitmap_sol_vector, sol2->bitmap_sol_vector, BITMAP_DYNAMIC_BYTE_SIZE(sol1))) {
    return false;
  }
  if(memcmp(sol1->sol_vector, sol2->sol_vector, sol1->last_zero * sizeof(*sol1->sol_vector))) {
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
	size_t i;
	for (i = 0; i < dest->dimension; i++) {
		set_inner_sol_element(dest, i, s1->sol_vector[i] + s2->sol_vector[i]);
	}
}

ssize_t find_idx_insertion(InnerSolution* sol_list, size_t sols_size, ssize_t
    lower_limit_idx, uint32_t value)
{
	size_t middle;
	size_t lower = lower_limit_idx + 1;
	size_t upper = sols_size - 1;

  if(lower_limit_idx >= 0 && sol_list[lower_limit_idx]->value < value) {
		return -1;
	}

	while (true) {
		middle = (lower + upper) / 2;
		if (sol_list[middle]->value >= value) {
			if (middle + 1 == sols_size || sol_list[middle + 1]->value < value) {
				return middle + 1;
			} else if (lower != middle) {
				lower = middle;
			} else {
				return -1;
			}
		} else if (sol_list[middle]->value < value) {
			if (middle == 0 || sol_list[middle - 1]->value >= value) {
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
    sols_size, InnerSolution* removed, ssize_t lower_limit_idx, uint32_t value, size_t K)
{
	uint32_t i;
	ssize_t idx = find_idx_insertion(sol_list, *sols_size, lower_limit_idx, value);

	if (idx == -1) {
		return -1;
	}

	if (*sols_size == K) {
	  if(removed == NULL) {
      kp_free_inn_sol(sol_list[K - 1]);
      sol_list[K - 1] = NULL;
    } else {
      *removed = sol_list[K - 1];
    }
	} else {
	  if(removed) {
	    *removed = NULL;
    }
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

  printf("\tLZ: %zu, [", sol->last_zero);
  for(i = 0; i < BITMAP_DYNAMIC_SIZE(sol); i++) {
    printf("%8llx", sol->bitmap_sol_vector[i]);
  }
  printf("]\n");
}
