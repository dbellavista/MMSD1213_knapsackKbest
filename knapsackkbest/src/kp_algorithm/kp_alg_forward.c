/**
 *
 * @file    kp_alg_forward.c
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    10/21/2013 01:26:09 PM
 * @brief   Implementation of the forward algorithms for solving the K-Best Knapsack problem
 *
 */

#include <stdlib.h>
#include "utility/debug.h"
#include "kp_algorithms.h"
#include "utility/utility.h"

void kp_build_initial_best_k_list(InnerSolution** ret, size_t* ret_size,
		int** matrix, KProblem problem, size_t K)
{

	uint32_t counter, P, P1;
	int snode, var, last_snode, last_var;
	bool fim, moreleft;
	InnerSolution** buffer_sol;
  InnerSolution* solutions;
  InnerSolution* solutions1;
  InnerSolution* tmp_solutions;
	InnerSolution* pIS;

	allocate_matrix((void***) &buffer_sol, 3, K, sizeof(InnerSolution));

	solutions = buffer_sol[0];
	solutions1 = buffer_sol[1];
	tmp_solutions = buffer_sol[2];


	*ret = (InnerSolution*) malloc(sizeof(InnerSolution) * K);

	fim = false;
	moreleft = false;
	counter = 0;
	// Adds a solution for each opened node (at most K)
	for (snode = problem->max_weigth - 1;
			snode >= (int) problem->weights[0] - 1; snode--) {
		for (var = problem->num_var - 1; var >= 0; var--) {
			if (matrix[snode][var] >= 0) {
				kp_init_inn_sol(&solutions[counter], problem->num_var, var,
						snode, matrix[snode][var]);
				counter++;
				if (counter == K) {
					last_snode = snode;
					last_var = var;
					moreleft = true;
					snode = -1;
					var = -1;
				}
			}
		}
	}
	P = counter;
	sort_by_values_non_inc(solutions, P);
	// Last usable matrix index: problem->weights[0] - 1
	if (P == K
			&& (last_snode > (int) problem->weights[0] - 1 || last_var > 0)) {
		fim = true;
	}

	// While there are more solutions to explore
	while (fim) {
		counter = 0;
		fim = false;
		// resuming from where it left off
		for (snode = last_snode; snode >= (int) problem->weights[0] - 1;
				snode--) {
			if (moreleft) {
				var = last_var - 1;
				moreleft = false;
			} else {
				var = problem->num_var - 1;
			}
			for (; var >= 0; var--) {
				// Check if the new value is better than last found solution
				if (matrix[snode][var] > (int) solutions[K - 1]->value) {
					kp_init_inn_sol(&solutions1[counter], problem->num_var, var,
							snode, matrix[snode][var]);
					counter++;
					if (counter == K) {
						last_snode = snode;
						last_var = var;
						moreleft = true;
						snode = -1;
						var = -1;
					}
				}
			}
		}

		P1 = counter;
		if (P1 > 0) {
		  // Joining with the new solutions
			sort_by_values_non_inc(solutions1, P1);
			if (solutions1[0]->value > solutions[K - 1]->value) {
				join_inner_solutions(tmp_solutions, solutions, solutions1, P,
						P1, K,
						true);
				pIS = solutions;
				solutions = tmp_solutions;
				tmp_solutions = pIS;
			}
			if (P1 == K
					&& (last_snode > (int) problem->weights[0] - 1 || last_var > 0)) {
				fim = true;
			}
		}
	}

	for (counter = 0; counter < P; counter++) {
		(*ret)[counter] = solutions[counter];
	}

	*ret_size = P;
	free_matrix((void**) buffer_sol);
}

void kp_forward_enumeration(int** matrix, KProblem problem)
{
	uint32_t var, snode_idx, var_idx;
	int value;

  // Matrix initialization
	for (snode_idx = 0; snode_idx < problem->max_weigth; snode_idx++) {
		for (var = 0; var < problem->num_var; var++) {
			matrix[snode_idx][var] = -1;
		}
	}

	// Begin initial ramification

	// The initial opened nodes are those with only one element
	for (var = 0; var < problem->num_var; var++) {
		if(problem->weights[var] <= problem->max_weigth) {
      matrix[problem->weights[var] - 1][var] = problem->values[var];
		}
	}

	// Begin ramification of supernodes (forward enumeration)
	// For each supernode, starting with the lowest feasible one,
	// if there's an opened node, it finds the best variable to add
	// to the current node.
	for (snode_idx = problem->weights[0] - 1;
			snode_idx < problem->max_weigth - problem->weights[0];
			snode_idx++) {
		// Find an opened supernode
		var_idx = problem->num_var;
		for (var = 0; var < problem->num_var; var++) {
			if (matrix[snode_idx][var] >= 0) {
				var_idx = var;
				break;
			}
		}
		// No opened node found
		if (var_idx == problem->num_var) {
			continue;
		}
		// Update values, that is, finds the best variable to add
		value = matrix[snode_idx][var_idx];
		for (var = var_idx; var < problem->num_var; var++) {
			// weight = (snode_idx + 1)
      if ((snode_idx + 1) + problem->weights[var] <= problem->max_weigth) {
				value = MAX(value, matrix[snode_idx][var]);
				matrix[snode_idx + problem->weights[var]][var] = value
						+ problem->values[var];
			}

		}
	}
}
