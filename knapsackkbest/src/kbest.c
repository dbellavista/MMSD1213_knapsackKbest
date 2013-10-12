/*
 * kbest.c
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */
#include <stdio.h>
#include <stdlib.h>
#include "kbest.h"
#include "utility.h"

void kp_init_sol(KSolution* solution, size_t n)
{
	*solution = (KSolution) malloc(sizeof(struct kSolution));
	(*solution)->vector_size = n;
	(*solution)->solution_vector = (uint32_t*) malloc(n * sizeof(uint32_t));
}

void kp_free_sol(KSolution solution) {
	free(solution->solution_vector);
	solution->solution_vector = NULL;
	free(solution);
}

void kp_init_kbest_sols(KBestSolutions* solution, KProblem problem, size_t sol_count)
{
	size_t i;
	*solution = (KBestSolutions) malloc(sizeof(struct kBestSolutions));
	kp_init_kp(&(*solution)->problem, problem->num_var, problem->weights, problem->values, problem->max_weigth);
	(*solution)->sol_count = sol_count;
	(*solution)->solutions = (KSolution *) malloc(
			sol_count * sizeof(KSolution));
	for (i = 0; i < sol_count; i++) {
		kp_init_sol(&(*solution)->solutions[i], problem->num_var);
	}

}
void kp_free_kbest_sols(KBestSolutions solution) {
	uint32_t i;
	kp_free_kp(solution->problem);
	solution->problem = NULL;
	for (i = 0; i < solution->sol_count; i++) {
		kp_free_sol(solution->solutions[i]);
	}
	free(solution->solutions);
	solution->solutions = NULL;
	free(solution);
}

void kp_init_kp(KProblem* problem, size_t n, uint32_t* weights, uint32_t* values, uint32_t max_w)
{
	uint32_t i;
	uint32_t* vals = (uint32_t*) malloc(n * 2 * sizeof(uint32_t));

	*problem = (KProblem) malloc(sizeof(struct kProblem));
	(*problem)->num_var = n;
	(*problem)->weights = vals;
	(*problem)->values = &(vals[n]);
	(*problem)->max_weigth = max_w;

	for (i = 0; i < n; i++) {
		(*problem)->weights[i] = weights[i];
		(*problem)->values[i] = values[i];
	}
	sort_by_weights(*problem);
}

void kp_free_kp(KProblem problem)
{
	free(problem->weights);
	problem->weights = NULL;
	problem->values = NULL;
	free(problem);
}

void kp_solve(KBestSolutions* dest, KProblem problem, size_t best_sol_count)
{
	InnerSolution* solutions;
	size_t solutions_size;
	int** matrix;

	// Matrix bxn initialization
	allocate_matrix((void ***) &matrix, problem->max_weigth, problem->num_var,
			sizeof(int));

	kp_forward_enumeration(matrix, problem);


	kp_build_initial_best_k_list(&solutions, &solutions_size, matrix, problem,
			best_sol_count);

	kp_recover_solution(solutions, solutions_size, best_sol_count, matrix,
			problem);

	create_kbest_solutions_from_inner(dest, solutions, solutions_size, problem, true);
}
