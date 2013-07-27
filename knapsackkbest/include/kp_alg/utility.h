/*
 * utility.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include "kp_alg.h"
#include "../kbest.h"

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

void join_inner_solutions(InnerSolution* dest, InnerSolution* sol1,
		InnerSolution* sol2, uint32 count1, uint32 count2,
		uint32 K, bool free_garbage);
void sort_by_values_non_inc(InnerSolution* sol, uint32 count);
void sort_by_weights(KProblem problem);
void allocate_matrix(void*** matrix, uint32 width, uint32 height, uint32 size);
void free_matrix(void** matrix);

int find_idx(uint32* vector, uint32 start, uint32 fin, uint32 value);

int find_idx_insertion(InnerSolution* sol_list, uint32 sols_size,
		uint32 limit_idx,
		uint32 value);

void sum_solution_vectors(InnerSolution dest, InnerSolution s1,
		InnerSolution s2);

#endif /* UTILITY_H_ */
