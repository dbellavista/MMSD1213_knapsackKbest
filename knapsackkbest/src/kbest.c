/*
 * kbest.c
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */
#include <stdio.h>
#include <stdlib.h>
#include "../include/kbest.h"
#include "../include/kp_alg/utility.h"

void kp_init_sol(KSolution* solution, uint32 n) {
	*solution = (KSolution) malloc(sizeof(struct kSolution));
	(*solution)->vector_size = n;
	(*solution)->solution_vector = (uint32*) malloc(n * sizeof(uint32));
}

void kp_free_sol(KSolution solution) {
	free(solution->solution_vector);
	free(solution);
}

void kp_init_kbest_sols(KBestSolutions* solution, KProblem problem,
uint32 sol_count) {
	*solution = (KBestSolutions) malloc(sizeof(struct kBestSolutions));
	(*solution)->problem = problem;
	(*solution)->sol_count = sol_count;
	uint32 i;
	(*solution)->solutions = (KSolution *) malloc(
			sol_count * sizeof(KSolution));
	for (i = 0; i < sol_count; i++) {
		kp_init_sol(&(*solution)->solutions[i], problem->num_var);
	}

}
void kp_free_kbest_sols(KBestSolutions solution) {
	uint32 i;

	for (i = 0; i < solution->sol_count; i++) {
		kp_free_sol(solution->solutions[i]);
	}
	free(solution->solutions);
	free(solution);
}

void kp_init_kp(KProblem* problem, uint32 n, uint32* weights, uint32* values,
uint32 max_w) {
	uint32 i;
	*problem = (KProblem) malloc(sizeof(struct kProblem));
	(*problem)->num_var = n;
	uint32* vals = (uint32*) malloc(n * 2 * sizeof(uint32));
	(*problem)->weights = vals;
	(*problem)->values = &(vals[n]);
	(*problem)->max_weigth = max_w;

	for (i = 0; i < n; i++) {
		(*problem)->weights[i] = weights[i];
		(*problem)->values[i] = values[i];
	}
	sort_by_weights(*problem);
}

void kp_free_kp(KProblem problem) {
	free(problem->weights);
	free(problem);
}

KBestSolutions kp_solve(KProblem problem, uint32 best_sol_count) {

	// Matrix bxn initialization
	int** matrix;
	allocate_matrix((void ***) &matrix, problem->max_weigth, problem->num_var,
			sizeof(int));

	kp_forward_enumeration(matrix, problem);

	InnerSolution* solutions;
	uint32 solutions_size;

	kp_build_initial_best_k_list(&solutions, &solutions_size, matrix, problem,
			best_sol_count);

	return NULL;
}
