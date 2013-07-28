/*
 * kp_alg.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef KP_ALG_H_
#define KP_ALG_H_

#include "../kbest.h"

struct innerSolution {
	uint32* sol_vector;
	uint32 dimension;
	uint32 row_idx;
	uint32 column_idx;
	uint32 value;
	bool recovered;
};
typedef struct innerSolution* InnerSolution;

/*
 * Initialize a knapsack inner solution.
 */
void kp_init_inn_sol(InnerSolution* innerSol, uint32 n, uint32 j, uint32 t, uint32 v);
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
void kp_build_initial_best_k_list(InnerSolution** ret, uint32* ret_size, int** matrix, KProblem problem, uint32 K);

/*
 *
 */
void kp_recover_solution(InnerSolution* solutions, uint32 size, uint32 K, int** matrix, KProblem problem);

/*
 *
 */
void backtracking(InnerSolution* solutions, InnerSolution dest, uint32 index, uint32 sols_size, uint32 K, int** matrix, KProblem problem);
/*
 *
 */
void search_alternative_solutions(uint32 row_idx, uint32 column_idx, uint32 cumul_value, uint32 j1, uint32 index, InnerSolution* solutions, uint32 sols_size, uint32 K, int** matrix, KProblem problem, uint32 last_var);

#endif /* KP_ALG_H_ */
