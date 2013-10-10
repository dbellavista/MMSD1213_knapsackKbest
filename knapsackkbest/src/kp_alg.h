/*
 * kp_alg.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef KP_ALG_H_
#define KP_ALG_H_

#include "kbest.h"

struct innerSolution {
	uint32_t* sol_vector;
	uint32_t dimension;
	uint32_t row_idx;
	uint32_t column_idx;
	uint32_t value;
	bool recovered;
};
typedef struct innerSolution* InnerSolution;

/*
 * Initialize a knapsack inner solution.
 */
void kp_init_inn_sol(InnerSolution* innerSol, uint32_t n, uint32_t j, uint32_t t, uint32_t v);
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
void kp_build_initial_best_k_list(InnerSolution** ret, uint32_t* ret_size, int** matrix, KProblem problem, uint32_t K);

/*
 *
 */
void kp_recover_solution(InnerSolution* solutions, uint32_t size, uint32_t K, int** matrix, KProblem problem);

/*
 *
 */
void backtracking(InnerSolution* solutions, InnerSolution dest, uint32_t index, uint32_t sols_size, uint32_t K, int** matrix, KProblem problem);
/*
 *
 */
void search_alternative_solutions(uint32_t row_idx, uint32_t column_idx, uint32_t cumul_value, uint32_t j1, uint32_t index, InnerSolution* solutions, uint32_t sols_size, uint32_t K, int** matrix, KProblem problem, uint32_t last_var);

#endif /* KP_ALG_H_ */
