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

#define uint8 unsigned char
#define uint16 unsigned short int
#define uint32 unsigned int
#define uint64 unsigned long long

#define byte uint8

#define int8 char
#define int16 short int
#define int64 long long

#define bool byte
#define false 0
#define true 1

struct kProblem {
	uint32 num_var;
	uint32* weights;
	uint32* values;
	uint32 max_weigth;
};
typedef struct kProblem* KProblem;

/*
 * Initialize a knapsack problem.
 */
void kp_init_kp(KProblem* problem, uint32 n, uint32* weights, uint32* values,
uint32 maxW);
/*
 * Free a knapsack problem.
 */
void kp_free_kp(KProblem problem);

struct kSolution {
	uint32 tot_value;
	uint32 vector_size;
	uint32* solution_vector;
};
typedef struct kSolution* KSolution;


struct kBestSolutions {
	KProblem problem;
	uint32 sol_count;
	KSolution* solutions;
};
typedef struct kBestSolutions* KBestSolutions;

/*
 * Initialize a knapsack solution.
 */
void kp_init_sol(KSolution* solution, uint32 n);
/*
 * Free a knapsack solution.
 */
void kp_free_sol(KSolution solution);

/*
 * Initialize a knapsack solution.
 */
void kp_init_kbest_sols(KBestSolutions* solution, KProblem problem, uint32 sol_count);
/*
 * Free a knapsack solution.
 */
void kp_free_kbest_sols(KBestSolutions solution);

/*
 * Solve the knapsack problem, finding best_sol_count solutions.
 */
KBestSolutions kp_solve(KProblem problem, uint32 best_sol_count);

#endif /* KBEST_H_ */
