/*
 * kbest.h
 *
 *  Created on: Jul 17, 2013
 *      Author: "Daniele Bellavista <daniele.bellavista@studio.unibo.it>
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

#define bool byte
#define false 0
#define true 1

struct kProblem {
	uint16 n;
	uint16* weights;
	uint16* values;
	uint16 max_w;
};
typedef struct kProblem* KProblem;

/*
 * Initialize a knapsack problem.
 */
void kp_init_kp(KProblem* problem, uint16 n, uint16* weights, uint16* values,
uint16 maxW);
/*
 * Free a knapsack problem.
 */
void kp_free_kp(KProblem problem);

struct kSolution {
	KProblem problem;
	uint16 sol_count;
	uint16** solutions;
};
typedef struct kSolution* KSolution;

/*
 * Initialize a knapsack solution.
 */
void kp_init_sol(KSolution* solution, KProblem problem, uint16 sol_count);
/*
 * Free a knapsack solution.
 */
void kp_free_sol(KSolution solution);

/*
 * Solve the knapsasck problem, finding best_sol_count solutions.
 */
KSolution kp_solve(KProblem problem, uint16 best_sol_count);

#endif /* KBEST_H_ */
