/*
 * utility.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include "data.h"
#include "kp_alg.h"
#include "kbest.h"

#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define ABS(a) \
  ((a < 0) ? -a : a)

#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define CEIL(x, TYPE) \
	( (x) >=0 ? (TYPE) ((x) + 1.0) : (TYPE) ((x) - 1.0))

#define FLOOR(x, TYPE) \
	( (TYPE) (x) )

#define ROUND(x, TYPE) \
	( (x) >=0 ? (TYPE) ((x) + 0.5) : (TYPE) ((x) - 0.5))

#define ROUND_LIMIT(x, LIMIT) \
  ((x) < 0 ? 0 : (x) >= (LIMIT) ? (LIMIT) - 1 : (unsigned long) ((x) + 0.5))

#define DOUBLE_EQUALS(x, y) \
	((x >= y - DBL_EPSILON) && (x <= y + DBL_EPSILON))

#define DZERO(x) DOUBLE_EQUALS(x, 0.0)

#define SQR(X) (X)*(X)

void create_kbest_solutions_from_inner(KBestSolutions* dest,
		InnerSolution* solutions, size_t solutions_size, KProblem problem,
		bool free_inner);

bool inner_solutions_equal(InnerSolution sol1, InnerSolution sol2);

void join_inner_solutions(InnerSolution* dest, InnerSolution* sol1,
		InnerSolution* sol2, size_t count1, size_t count2,
		size_t K, bool free_garbage);
void sort_by_values_non_inc(InnerSolution* sol, uint32_t count);
void sort_by_weights(KProblem problem);
void allocate_matrix(void*** matrix, uint32_t nrow, uint32_t ncols, uint32_t size);
void free_matrix(void** matrix);

ssize_t find_idx(int* vector, size_t fin, int value);

ssize_t find_idx_insertion(InnerSolution* sol_list, size_t sols_size,
    ssize_t lower_limit_idx, uint32_t value);

ssize_t find_idx_and_prepare_insertion(InnerSolution* sol_list, size_t*
    sols_size, InnerSolution* removed, ssize_t lower_limit_idx, uint32_t value, size_t K);

void sum_solution_vectors(InnerSolution dest, InnerSolution s1, InnerSolution
    s2);

void print_inner_solution(InnerSolution sol);

void set_inner_sol_element(InnerSolution isol, size_t var, uint32_t value);

#endif /* UTILITY_H_ */
