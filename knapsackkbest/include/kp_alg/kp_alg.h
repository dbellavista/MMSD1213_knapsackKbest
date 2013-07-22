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
	uint16* sol_vector;
	uint16 dimension;
	uint16 row_idx;
	uint16 column_idx;
	uint16 value;
	bool recovered;
};
typedef struct innerSolution* InnerSolution;

/*
 * Initialize a knapsack inner solution.
 */
void kp_init_inn_sol(InnerSolution* innerSol, uint16 n, uint16 j, uint16 t, uint16 v);
/*
 * Free a knapsack inner solution.
 */
void kp_free_inn_sol(InnerSolution innerSol);

/*
 * Performs the forward enumeration on the given problem, filling the matrix.
 */
void kp_forward_enumeration(uint16** matrix, KProblem problem);

/*
 *
 */
void kp_build_initial_best_k_list(InnerSolution** ret, uint16* ret_size, uint16** matrix, KProblem problem, uint16 K);

/*
 *
 */
void recover_solution(InnerSolution* solutions, uint16 size, uint16 K, uint16** matrix, KProblem problem);

/*
 *
 */
void backtracking(InnerSolution* solutions, InnerSolution dest, uint16 index, uint16 sols_size, uint16 K, uint16** matrix, KProblem problem);

#endif /* KP_ALG_H_ */
