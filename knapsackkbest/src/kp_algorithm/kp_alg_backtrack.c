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

/**
 *  @brief       Try to find a solution in \p solutions that has the same
 *               solution vector of \p sol
 *
 *  @param[in]   solutions   The array of solutions to iterate.
 *  @param[in]   sols_size   The size of \p solutions.
 *  @param[in]   sol   The solution to search.
 *  @param[in]   pivot The index to start with
 *  @param[in]   exclude_pivot True if the pivot must be escluded from search
 *
 *  @return      Returns the index of the duplicate solution if exists, -1 otherwise.
 *
 *  @details     A search for an alternative solution could lead to a solution
 *               which solution vector is equals to the one of an another solution.
 */
ssize_t find_duplicate(InnerSolution* solutions, size_t sols_size,
    InnerSolution sol, size_t pivot, bool exclude_pivot);

ssize_t find_duplicate(InnerSolution* solutions, size_t sols_size,
    InnerSolution sol, size_t pivot, bool exclude_pivot)
{
  InnerSolution tmp_sol;
  ssize_t i;
  size_t j;
  i = (exclude_pivot) ? pivot - 1 : pivot;
  for(; i >= 0; i--) {
    tmp_sol = solutions[i];
    if(tmp_sol->value > sol->value) {
      break;
    }
    if(tmp_sol->value == sol->value && inner_solutions_equal(tmp_sol, sol)) {
      return i;
    }
  }

  j = pivot + 1;
  for(; j < sols_size; j++) {
    tmp_sol = solutions[j];
    if(tmp_sol->value < sol->value) {
      break;
    }
    if(tmp_sol->value == sol->value && inner_solutions_equal(tmp_sol, sol)) {
      return i;
    }
  }

  return -1;
}

void search_alternative_solutions(size_t snode, size_t cur_var, uint32_t
    cum_val, size_t limit_var, size_t sol_index, InnerSolution auxl,
    InnerSolution* solutions, size_t* sols_size, size_t K, int** matrix,
    KProblem problem)
{

	ssize_t insert_idx, i;
	size_t var;
  uint32_t newvalue;
	InnerSolution auxl1, tmp_sol;

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
		insert_idx = find_idx_insertion(solutions, *sols_size, K, sol_index, newvalue);
		if (insert_idx == -1) {
      // Already have K solutions better than the new value
			continue;
		}
		kp_init_inn_sol(&auxl1, problem->num_var, var, snode,
				matrix[snode][var]);

		// Backtrack the new value in order to recover the solution vector
		backtracking(solutions, auxl1, insert_idx, sols_size, K, matrix,
				problem, false);
    // Initialize the new solution
    kp_init_inn_sol(&tmp_sol, problem->num_var,
        solutions[sol_index]->column_idx, solutions[sol_index]->row_idx,
        newvalue);
    // Sum the solutions vector of the current solution so far and the recovered solution
    sum_solution_vectors(tmp_sol, auxl, auxl1);
    tmp_sol->recovered = true;

    // Auxl1 is recovered. Checks if the new solution is a duplicate. If not
    // insert it in the solution vector.
    i = find_duplicate(solutions, *sols_size, tmp_sol, insert_idx, false);

    if(i < 0) {
      // Inserting the new solution
      prepare_insertion(solutions, sols_size, insert_idx, NULL, K);
      solutions[insert_idx] = tmp_sol;
      tmp_sol = NULL;
    } else {
      // Duplicat found! Freeing the new solution
      kp_free_inn_sol(tmp_sol);
      tmp_sol = NULL;
    }

    // Checking if the solution auxl1 is worth to be inserted
		if (auxl1->value >= solutions[*sols_size - 1]->value) {
		  auxl1->recovered = 1;
			// Auxl1 can be a duplicate too!
			if(find_duplicate(solutions, *sols_size, auxl1, *sols_size - 1, false) == -1) {
        insert_idx = find_idx_and_prepare_insertion(solutions, sols_size, NULL,
            insert_idx, matrix[snode][var], K);
        if (insert_idx == -1) {
          d_error(
              "Cannot find an idx for second insertion! Value: %d, SolIndex: %d\n",
              matrix[snode][var], insert_idx);
        } else {
          solutions[insert_idx] = auxl1;
          auxl1 = NULL;
        }
      }
		}

		if(auxl1)
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

	cum_val = 0;
	// Starting from the solution supernode, removes the current variable
	// and add it to the solution vector
	while (1) {

		value -= problem->values[var];
		cum_val += problem->values[var];
		set_inner_sol_element(sol_dest, var, sol_dest->sol_vector[var] + 1);

		limit_var = sol_dest->column_idx;

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

	}
	sol_dest->recovered = true;

	d_dec_indent();
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
