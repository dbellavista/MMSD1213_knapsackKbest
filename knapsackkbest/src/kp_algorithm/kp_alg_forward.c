/**
 *
 * @file    kp_alg_forward.c
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    10/21/2013 01:26:09 PM
 * @brief   Implementation of the forward algorithms for solving the K-Best Knapsack problem
 *
 */

#include <stdlib.h>
#include "kbest_io_utility.h"
#include "kp_algorithms.h"
#include "utility/utility.h"

void kp_build_initial_best_k_list(InnerSolution** ret, size_t* ret_size,
		int** matrix, KProblem problem, size_t K)
{
	ssize_t snode, var, idx;
	size_t counter;
	*ret = (InnerSolution*) malloc(sizeof(InnerSolution) * K);
	counter = 0;

	for (snode = problem->max_weigth - 1;
			snode >= problem->weights[0] - 1; snode--) {
		for (var = problem->num_var - 1; var >= 0; var--) {
			if (matrix[snode][var] >= 0) {
        idx = find_idx_and_prepare_insertion(*ret, &counter, -1, matrix[snode][var], K);
        if(idx >= 0) {
          kp_init_inn_sol(&(*ret)[idx], problem->num_var, var, snode, matrix[snode][var]);
        }
      }
    }
  }
  *ret_size = counter;
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
