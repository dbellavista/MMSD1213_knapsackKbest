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

void kp_init_sol(KSolution* solution, KProblem problem, uint16 sol_count) {
	*solution = (KSolution) malloc(sizeof(struct kSolution));
	(*solution)->problem = problem;
	(*solution)->sol_count = sol_count;

	allocate_matrix((void***) &((*solution)->solutions), sol_count, problem->n,
			sizeof(uint16));
}

void kp_free_sol(KSolution solution) {
	free(solution->solutions[0]);
	free(solution->solutions);
	free(solution);
}

void kp_init_kp(KProblem* problem, uint16 n, uint16* weights, uint16* values,
uint16 max_w) {
	uint16 i;
	*problem = (KProblem) malloc(sizeof(struct kProblem));
	(*problem)->n = n;
	uint16* vals = (uint16*) malloc(n * 2 * sizeof(uint16));
	(*problem)->weights = vals;
	(*problem)->values = &(vals[n]);
	(*problem)->max_w = max_w;

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

KSolution kp_solve(KProblem problem, uint16 best_sol_count) {
	uint16 i,j;
	uint16** matrix;
	// Matrix bxn initialization
	allocate_matrix((void ***) &matrix, problem->max_w, problem->n,
			sizeof(uint16));

	for(i = 0; i < problem->max_w; i++) {
		for(j = 0; j < problem->n; j++) {
			matrix[i][j] = -1;
		}
	}

	return NULL;
}
