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

void join_inner_solutions(InnerSolution** dest, InnerSolution* sol1,
		InnerSolution* sol2, uint16 count1, uint16 count2);
void sort_by_values_non_inc(InnerSolution* sol, uint16 count);
void sort_by_weights(KProblem problem);
void allocate_matrix(void*** matrix, uint32 width, uint32 height, uint32 size);
void free_matrix(void** matrix);

#endif /* UTILITY_H_ */
