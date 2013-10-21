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
#include "KBestConfig.h"

/**
 *  @brief       Try to find a solution in \p solutions that has the same
 *               solution vector of \p sol
 *
 *  @param[in]   solutions   The array of solutions to iterate.
 *  @param[in]   sols_size   The size of \p solutions.
 *  @param[in]   sol   The solution to search.
 *  @param[in]   excluded The index to skip (usually the index of \p sol inside \p solutions).
 *
 *  @return      Returns the index of the duplicate solution if exists, -1 otherwise.
 *
 *  @details     A search for an alternative solution could lead to a solution
 *               which solution vector is equals to the one of an another solution.
 */
ssize_t find_duplicate(InnerSolution* solutions, size_t sols_size,
    InnerSolution sol, size_t excluded);

ssize_t find_duplicate(InnerSolution* solutions, size_t sols_size,
    InnerSolution sol, size_t excluded)
{
  InnerSolution tmp_sol;
  size_t i;

  for(i = 0; i < sols_size; i++) {
    if(i == excluded) {
      continue;
    }
    tmp_sol = solutions[i];
    if(tmp_sol->value < sol->value) {
      return -1;
    }
    if(inner_solutions_equal(tmp_sol, sol)) {
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
	InnerSolution auxl1, removed;

#ifdef ENABLE_DEBUG_PRINTS
  d_inc_indent();
#endif

  // From the current node, trying to find an alternative solution by adding
  // different (feasible) variables
  var = limit_var;
	for(var = limit_var; var + 1 != 0; var--) {
#ifdef ENABLE_DEBUG_PRINTS
    d_debug(
        "Alternatives %d (%d): Start: (%d, %d). Limit: %d. Trying position (%d, %d) -> value: %d\n",
        sol_index, solutions[sol_index]->value, snode, cur_var,
        limit_var, snode, var, matrix[snode][var]);
#endif

		if (var == cur_var || matrix[snode][var] < 0) {
#ifdef ENABLE_DEBUG_PRINTS
      d_debug(
          "Alternatives %d (%d): Start: (%d, %d). Skipping position (%d, %d)\n",
          sol_index, solutions[sol_index]->value, snode, cur_var,
          snode, var);
#endif
			continue;
		}

		newvalue = matrix[snode][var] + cum_val;
		if (newvalue < solutions[*sols_size - 1]->value) {
#ifdef ENABLE_DEBUG_PRINTS
      d_debug(
          "alternatives %d (%d): start: (%d, %d). skipping position (%d, %d) (value %d ok but not worth).\n",
          sol_index, solutions[sol_index]->value, snode, cur_var,
          snode, var, newvalue);
#endif
			continue;
		}
		// Newvalue is worth to be backtracked!
		
#ifdef ENABLE_DEBUG_PRINTS
    d_debug(
        "Alternatives %d (%d): new solution from position (%d, %d): %d by adding cum value %d to %d\n",
        sol_index, solutions[sol_index]->value, snode, var, newvalue,
        cum_val, matrix[snode][var]);
#endif

    // Find a suitable position for the solution insertion
		insert_idx = find_idx_and_prepare_insertion(solutions, sols_size, &removed, sol_index,
				newvalue, K);
		if (insert_idx == -1) { // Already have K solutions better than the new value
#ifdef ENABLE_DEBUG_PRINTS
      d_debug(
          "Alternatives %d (%d): new solution %d not inserted, value equals to last solution and solution vector full.\n",
          sol_index, solutions[sol_index]->value, newvalue);
#endif
			continue;
		}
		kp_init_inn_sol(&solutions[insert_idx], problem->num_var,
				solutions[sol_index]->column_idx, solutions[sol_index]->row_idx,
				newvalue);

		kp_init_inn_sol(&auxl1, problem->num_var, var, snode,
				matrix[snode][var]);

#ifdef ENABLE_DEBUG_PRINTS
    d_debug(
        "alternatives %d (%d): inserting the new solution (%d) in position %d. now backtraking\n",
        sol_index, solutions[sol_index]->value, newvalue, insert_idx);
#endif
		// Backtrack the new value in order to fill the solution vector
		backtracking(solutions, auxl1, insert_idx, sols_size, K, matrix,
				problem, false);

		solutions[insert_idx]->recovered = true;

    // TODO : changed instead of using solutions[sol_index], using auxl (passed from backtrack)
//		sum_solution_vectors(solutions[insert_idx], solutions[sol_index],
//				auxl1);
    sum_solution_vectors(solutions[insert_idx], auxl, auxl1);

    i = find_duplicate(solutions, *sols_size, solutions[insert_idx], insert_idx);

    if(i >= 0) {
#ifdef ENABLE_DEBUG_PRINTS
      d_debug(
          "Alternatives %d. New solution [%zu %u] is a duplicate of solution [%zu %u].\n",
          sol_index, insert_idx, solutions[insert_idx]->value, i, solutions[i]->value);
#endif

      // removing the solution and inserting the old one
      kp_free_inn_sol(solutions[insert_idx]);
      solutions[insert_idx] = NULL;
      for(i = insert_idx + 1; (size_t) i < *sols_size; i++) {
        solutions[i - 1] = solutions[i];
      }
      *sols_size = *sols_size - 1;

      if(removed) {
        insert_idx = find_idx_and_prepare_insertion(solutions, sols_size, NULL,
            insert_idx - 1, removed->value, K);
        if(insert_idx == -1) {
#ifdef ENABLE_DEBUG_PRINTS
          d_debug(
              "Alternatives %d. Removed value no longer worth of insertion (%d)!\n",
              sol_index, removed->value);
#endif
        } else {
#ifdef ENABLE_DEBUG_PRINTS
          d_debug(
              "Alternatives %d. Restoring previously removed value (%d)!\n",
              sol_index, removed->value);
#endif
          solutions[insert_idx] = removed;
          removed = NULL;
        }
      }
    }

    if(removed)
      kp_free_inn_sol(removed);

		if (auxl1->value >= solutions[*sols_size - 1]->value) {
		  // TODO: check if this is right
		  auxl1->recovered = 1;
#ifdef ENABLE_DEBUG_PRINTS
      d_debug(
          "alternatives %d. partial solution in (%d, %d) is better than last value (%d >= %d)\n",
          sol_index, snode, cur_var, auxl1->value, solutions[*sols_size - 1]->value);
#endif

			if(find_duplicate(solutions, *sols_size, auxl1, -1)) {
#ifdef ENABLE_DEBUG_PRINTS
        d_debug(
            "Alternatives %d. Partial solution in (%d, %d) is already present!\n",
            sol_index, snode, cur_var);
#endif
      } else {
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

#ifdef ENABLE_DEBUG_PRINTS
  d_dec_indent();
#endif
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

#ifdef ENABLE_DEBUG_PRINTS
  d_inc_indent();
#endif
#ifdef ENABLE_DEBUG_PRINTS
  d_debug("Backtracking %d (%d): [%d in (%d, %d)]\n", sol_index,
      solutions[sol_index]->value, value, snode, var);
#endif

	cum_val = 0;
	// Starting from the solution supernode, removes the current variable
	// and add it to the solution vector
	while (1) {

		value -= problem->values[var];
		cum_val += problem->values[var];
		set_inner_sol_element(sol_dest, var, sol_dest->sol_vector[var] + 1);

		limit_var = sol_dest->column_idx;

		if (snode + 1 == problem->weights[var]) { // Backtrack terminated
#ifdef ENABLE_DEBUG_PRINTS
      d_debug("Backtracking %d (%d): terminated with var %d\n", sol_index,
          solutions[sol_index]->value, var);
#endif
			break;
		} else if(snode < problem->weights[var]) { // Algorithm error
			d_error("Error while backtracking %d (%d): terminated with var %d\n", sol_index,
					solutions[sol_index]->value, var);
			break;
    }
		snode -= problem->weights[var];

		// Finds the index of the value: it must be in the matrix
		tmp = find_idx(matrix[snode], var, (int) value);

		if (tmp < 0) { // Algorithm error
			d_error("Backtracking %d (%d): NOT Found in snode %d, value %d\n",
					sol_index, solutions[sol_index]->value, snode, value);
			continue;
		}

#ifdef ENABLE_DEBUG_PRINTS
    d_debug("Backtracking %d (%d): "
            "Found [%d in (%d, %d)] by subtracting var %d "
            "(w:%d, v:%d), cum_val: %d, limit: %d\n",
        sol_index, solutions[sol_index]->value, value, snode, tmp,
        var, problem->weights[var], problem->values[var], cum_val,
        limit_var);
#endif

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
	uint32_t i, steps = (*size) / 100 + 1;
	// For each solution, backtrack
	for (i = 0; i < *size; i++) {
#ifdef ENABLE_DEBUG_PRINTS
    d_debug("recovering for solution %d [%d in (%d, %d)]\n", i,
        solutions[i]->value, solutions[i]->row_idx,
        solutions[i]->column_idx);
#endif

		if(i % steps == 0) {
      d_notice("Recovering for solution %d of %d\n", i + 1, *size);
    }

		if (!solutions[i]->recovered) {

			kp_copy_inn_sol(&auxl, solutions[i]);
			backtracking(solutions, auxl, i, size, K, matrix, problem, true);
			kp_free_inn_sol(solutions[i]);
			solutions[i] = auxl;

		}
	}

}
