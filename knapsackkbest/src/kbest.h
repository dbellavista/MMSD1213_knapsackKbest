/*
 * kbest.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 *
 * This file contains the user interface for solving the kp problem.
 *
 */

#ifndef KBEST_H_
#define KBEST_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

struct kProblem {
	size_t num_var;
	uint32_t* weights;
	uint32_t* values;
	uint32_t max_weigth;
};
typedef struct kProblem* KProblem;

/*
 * Initialize a knapsack problem.
 */
void kp_init_kp(KProblem* problem, size_t n, uint32_t* weights, uint32_t*
    values, uint32_t maxW);
/*
 * Free a knapsack problem.
 */
void kp_free_kp(KProblem problem);

struct kSolution {
	uint32_t tot_value;
	size_t vector_size;
	uint32_t* solution_vector;
};
typedef struct kSolution* KSolution;


struct kBestSolutions {
	KProblem problem;
	size_t sol_count;
	KSolution* solutions;
};
typedef struct kBestSolutions* KBestSolutions;

/*
 * Initialize a knapsack solution.
 */
void kp_init_sol(KSolution* solution, size_t n);
/*
 * Free a knapsack solution.
 */
void kp_free_sol(KSolution solution);

/*
 * Initialize a knapsack solution.
 */
void kp_init_kbest_sols(KBestSolutions* solution, KProblem problem, size_t sol_count);
/*
 * Free a knapsack solution.
 */
void kp_free_kbest_sols(KBestSolutions solution);

/*
 * Solve the knapsack problem, finding best_sol_count solutions.
 */
void kp_solve(KBestSolutions* dest, KProblem problem, size_t best_sol_count);

#endif /* KBEST_H_ */
