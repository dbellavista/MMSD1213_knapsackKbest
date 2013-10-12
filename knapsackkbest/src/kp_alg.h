/*
 * kp_alg.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef KP_ALG_H_
#define KP_ALG_H_

#include <limits.h>

#include "kbest.h"

#if defined(__x86_64__) || defined(_M_X64)
typedef unsigned long long bitmap_t;
#else
typedef unsigned long bitmap_t;
#endif


#define BITMAP_DYNAMIC_BYTE_SIZE(sol) CEIL((sol)->dimension / 8, size_t)
#define BITMAP_DYNAMIC_SIZE(sol) BITMAP_DYNAMIC_BYTE_SIZE(sol) / sizeof(bitmap_t)

#define SET_SOL_ELEMENT(iSol, var, value)\
  iSol->sol_vector[var] = value;\
  if(value) iSol->bitmap_sol_vector[var / (8 * sizeof(bitmap_t))] |= 1 << (var % (8 * sizeof(bitmap_t))); \
  if (var > iSol->last_zero) iSol->last_zero = var

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

/*
 * Performs the forward enumeration on the given problem, filling the matrix.
 */
void kp_forward_enumeration(int** matrix, KProblem problem);

/*
 *
 */
void kp_build_initial_best_k_list(InnerSolution** ret, size_t* ret_size, int** matrix, KProblem problem, size_t K);

/*
 *
 */
void kp_recover_solution(InnerSolution* solutions, size_t size, size_t K, int** matrix, KProblem problem);

/*
 *
 */
void backtracking(InnerSolution* solutions, InnerSolution dest, size_t index, size_t sols_size, size_t K, int** matrix, KProblem problem);
/*
 *
 */
void search_alternative_solutions(size_t row_idx, size_t column_idx, uint32_t cumul_value, size_t j1, size_t index, InnerSolution* solutions, size_t sols_size, size_t K, int** matrix, KProblem problem, size_t last_var);

#endif /* KP_ALG_H_ */
