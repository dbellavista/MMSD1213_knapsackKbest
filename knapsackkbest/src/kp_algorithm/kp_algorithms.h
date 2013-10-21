/*
 * kp_alg.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef KP_ALG_H_
#define KP_ALG_H_

#include "kp_model/kp_inner_model.h"
#include "kbest.h"

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
void kp_recover_solution(InnerSolution* solutions, size_t* size, size_t K, int** matrix, KProblem problem);

/*
 *
 */
void backtracking(InnerSolution* solutions, InnerSolution dest, size_t index, size_t* sols_size, size_t K, int** matrix, KProblem problem, bool alternative);
/*
 *
 */
void search_alternative_solutions(size_t snode, size_t cur_var, uint32_t
    cum_val, size_t limit_var, size_t sol_index, InnerSolution auxl,
    InnerSolution* solutions, size_t* sols_size, size_t K, int** matrix,
    KProblem problem, size_t last_var);

#endif /* KP_ALG_H_ */
