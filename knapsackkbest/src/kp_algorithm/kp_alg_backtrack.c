/**
 *
 * @file    kp_alg_backtrack.c
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    10/21/2013 01:26:09 PM
 * @brief   Implementation of the backtracking algorithms for solving the K-Best Knapsack problem
 *
 */

#include <stdlib.h>
#include "kbest_io_utility.h"
#include "kp_algorithms.h"
#include "utility/utility.h"

void search_alternative_solutions(size_t snode, size_t cur_var, uint32_t
    cum_val, size_t limit_var, size_t sol_index, InnerSolution auxl,
    InnerSolution* solutions, size_t* sols_size, size_t K, int** matrix,
    KProblem problem)
{

	ssize_t insert_idx;
	size_t var;
  uint32_t newvalue;
	InnerSolution auxl1;

  // From the current node, trying to find an alternative solution by adding
  // different (feasible) variables
  var = limit_var;
	for(var = limit_var; var + 1 != 0; var--) {
    // Exploting matrix[snode][var]
		if (var == cur_var || matrix[snode][var] < 0) {
		  // Skipping where matrix is -1 and var == cur_var
			continue;
		}

		newvalue = matrix[snode][var] + cum_val;
		if (newvalue < solutions[*sols_size - 1]->value) {
		  // The new value is not worth (the last solution is better)
			continue;
		}
		// Newvalue is worth to be backtracked!
    // Finding a suitable position for the solution insertion
		insert_idx = find_idx_and_prepare_insertion(solutions, sols_size, sol_index, newvalue, K);
		if (insert_idx == -1) {
      // Already have K solutions better than the new value
			continue;
		}
		kp_init_inn_sol(&auxl1, problem->num_var, var, snode,
				matrix[snode][var]);
    // Initialize the new solution
    kp_init_inn_sol(&solutions[insert_idx], problem->num_var,
        solutions[sol_index]->column_idx, solutions[sol_index]->row_idx,
        newvalue);

		// Backtrack the new value in order to recover the solution vector
		backtracking(solutions, auxl1, insert_idx, sols_size, K, matrix,
				problem, false);
    // Sum the solutions vector of the current solution so far and the recovered solution
    sum_solution_vectors(solutions[insert_idx], auxl, auxl1);
    solutions[insert_idx]->recovered = true;

    kp_free_inn_sol(auxl1);
	}
}

void backtracking(InnerSolution* solutions, InnerSolution sol_dest, size_t
    sol_index, size_t* sols_size, size_t K, int** matrix, KProblem problem, bool alternative)
{
	ssize_t tmp;
	size_t snode, var, limit_var;
	uint32_t value, cum_val;

  snode = sol_dest->row_idx;
  var = sol_dest->column_idx;
  value = sol_dest->value;
	limit_var = sol_dest->column_idx;

	cum_val = 0;
	// Starting from the solution supernode, removes the current variable
	// and add it to the solution vector
	while (1) {

		value -= problem->values[var];
		cum_val += problem->values[var];
		sol_dest->sol_vector[var] = sol_dest->sol_vector[var] + 1;

		if (snode + 1 == problem->weights[var]) { // Backtrack terminated
			break;
		} else if(snode < problem->weights[var]) { // Algorithm error
			d_error("Error while backtracking %d (%d): terminated with var %d\n", sol_index,
					sol_dest->value, var);
			break;
    }
		snode -= problem->weights[var];

		// Finds the index of the value: it must be in the matrix
		tmp = find_idx(matrix[snode], var, (int) value);

		if (tmp < 0) { // Algorithm error
			d_error("Backtracking %d (%d): NOT Found in snode %d, value %d\n",
					sol_index, sol_dest->value, snode, value);
			continue;
		}

    var = (size_t) tmp;
    if(alternative)
      search_alternative_solutions(snode, var, cum_val, limit_var,
          sol_index, sol_dest, solutions, sols_size, K, matrix, problem);

    // This is the cause of everything!
    limit_var = var;

	}
	sol_dest->recovered = true;
}

void kp_recover_solution(InnerSolution* solutions, size_t* size, size_t K,
    int** matrix, KProblem problem)
{
	InnerSolution auxl;
	uint32_t i;
	// For each solution, backtrack
	for (i = 0; i < *size; i++) {
		if (!solutions[i]->recovered) {
			kp_copy_inn_sol(&auxl, solutions[i]);
			backtracking(solutions, auxl, i, size, K, matrix, problem, true);
			kp_free_inn_sol(solutions[i]);
			solutions[i] = auxl;
		}
	}
}
