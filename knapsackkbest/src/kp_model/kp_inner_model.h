/*
 * kp_alg.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef KP_INNER_MODEL_H_
#define KP_INNER_MODEL_H_

#include <limits.h>

#include "kbest.h"

#if defined(__x86_64__) || defined(_M_X64)
typedef unsigned long long bitmap_t;
#else
typedef unsigned long bitmap_t;
#endif


#define BITMAP_DYNAMIC_BYTE_SIZE(sol) CEIL((sol)->dimension / 8, size_t)
#define BITMAP_DYNAMIC_SIZE(sol) CEIL(BITMAP_DYNAMIC_BYTE_SIZE(sol) / sizeof(bitmap_t), size_t)

struct innerSolution {
	uint32_t* sol_vector;
	size_t dimension;
	size_t row_idx;
	size_t column_idx;
	uint32_t value;
	bool recovered;

  bitmap_t *bitmap_sol_vector;
  size_t last_zero;
};

typedef struct innerSolution* InnerSolution;

/*
 * Initialize a knapsack inner solution.
 */
void kp_init_inn_sol(InnerSolution* innerSol, size_t n, size_t j, size_t t, uint32_t v);
/*
 * Free a knapsack inner solution.
 */
void kp_free_inn_sol(InnerSolution innerSol);
/*
 * Create a copy of an innersolution
 */
void kp_copy_inn_sol(InnerSolution* dest, InnerSolution origin);

void create_kbest_solutions_from_inner(KBestSolutions* dest, InnerSolution*
    solutions, size_t solutions_size, KProblem problem, bool free_inner);

bool inner_solutions_equal(InnerSolution sol1, InnerSolution sol2);

void join_inner_solutions(InnerSolution* dest, InnerSolution* sol1,
    InnerSolution* sol2, size_t count1, size_t count2, size_t K, bool
    free_garbage);

void sort_by_values_non_inc(InnerSolution* sol, uint32_t count);

ssize_t find_idx_insertion(InnerSolution* sol_list, size_t sols_size, ssize_t
    lower_limit_idx, uint32_t value);

ssize_t find_idx_and_prepare_insertion(InnerSolution* sol_list, size_t*
    sols_size, InnerSolution* removed, ssize_t lower_limit_idx, uint32_t value,
    size_t K);

void sum_solution_vectors(InnerSolution dest, InnerSolution s1, InnerSolution
    s2);

void print_inner_solution(InnerSolution sol);

void set_inner_sol_element(InnerSolution isol, size_t var, uint32_t value);

#endif /* KP_INNER_MODEL_H_ */
