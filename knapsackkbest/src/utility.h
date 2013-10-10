/*
 * utility.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include "kp_alg.h"
#include "kbest.h"

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

void create_kbest_solutions_from_inner(KBestSolutions* dest,
		InnerSolution* solutions, uint32_t solutions_size, KProblem problem,
		bool free_inner);

void join_inner_solutions(InnerSolution* dest, InnerSolution* sol1,
		InnerSolution* sol2, uint32_t count1, uint32_t count2,
		uint32_t K, bool free_garbage);
void sort_by_values_non_inc(InnerSolution* sol, uint32_t count);
void sort_by_weights(KProblem problem);
void allocate_matrix(void*** matrix, uint32_t nrow, uint32_t ncols, uint32_t size);
void free_matrix(void** matrix);

int find_idx(int* vector, uint32_t fin, int value);

int find_idx_insertion(InnerSolution* sol_list, uint32_t sols_size,
uint32_t lower_limit_idx,
uint32_t value);

int find_idx_and_prepare_insertion(InnerSolution* sol_list, uint32_t* sols_size,
uint32_t lower_limit_idx, uint32_t value, uint32_t K);

void sum_solution_vectors(InnerSolution dest, InnerSolution s1,
		InnerSolution s2);

#endif /* UTILITY_H_ */
