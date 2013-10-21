/*
 * kp_alg.c
 *
 *  Created on: Jul 18, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdlib.h>
#include "utility/debug.h"
#include "kp_algorithms.h"
#include "utility/utility.h"

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
    KProblem problem, size_t last_var)
{

	ssize_t insert_idx, i;
	size_t var;
  uint32_t newvalue;
	InnerSolution auxl1, removed;

//	d_inc_indent();

  // From the current node, trying to find an alternative solution by adding
  // different (feasible) variables
  var = limit_var;
	for(var = limit_var; var + 1 != 0; var--) {
//		d_debug(
//				"Alternatives %d (%d): Start: (%d, %d). Limit: %d. Trying position (%d, %d) -> value: %d\n",
//				sol_index, solutions[sol_index]->value, snode, cur_var,
//				limit_var, snode, var, matrix[snode][var]);

		if (var == cur_var || matrix[snode][var] < 0) {
//			d_debug(
//					"Alternatives %d (%d): Start: (%d, %d). Skipping position (%d, %d)\n",
//					sol_index, solutions[sol_index]->value, snode, cur_var,
//					snode, var);
			continue;
		}

		newvalue = matrix[snode][var] + cum_val;
		if (newvalue < solutions[*sols_size - 1]->value) {
//			d_debug(
//					"Alternatives %d (%d): Start: (%d, %d). Skipping position (%d, %d) (value %d ok but not worth).\n",
//					sol_index, solutions[sol_index]->value, snode, cur_var,
//					snode, var, newvalue);
			continue;
		}
		// Newvalue is worth to be backtracked!
		
//		d_debug(
//				"Alternatives %d (%d): new solution from position (%d, %d): %d by adding cum value %d to %d\n",
//				sol_index, solutions[sol_index]->value, snode, var, newvalue,
//				cum_val, matrix[snode][var]);

    // Find a suitable position for the solution insertion
		insert_idx = find_idx_and_prepare_insertion(solutions, sols_size, &removed, sol_index,
				newvalue, K);
		if (insert_idx == -1) { // Already have K solutions better than the new value
//			d_debug(
//					"Alternatives %d (%d): new solution %d not inserted, value equals to last solution and solution vector full.\n",
//					sol_index, solutions[sol_index]->value, newvalue);
			continue;
		}
		kp_init_inn_sol(&solutions[insert_idx], problem->num_var,
				solutions[sol_index]->column_idx, solutions[sol_index]->row_idx,
				newvalue);

		kp_init_inn_sol(&auxl1, problem->num_var, var, snode,
				matrix[snode][var]);

//		d_debug(
//				"Alternatives %d (%d): inserting the new solution (%d) in position %d. Now Backtraking\n",
//				sol_index, solutions[sol_index]->value, newvalue, insert_idx);
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
//      d_debug(
//          "Alternatives %d. New solution [%zu %u] is a duplicate of solution [%zu %u].\n",
//          sol_index, insert_idx, solutions[insert_idx]->value, i, solutions[i]->value);

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
//          d_debug(
//              "Alternatives %d. Removed value no longer worth of insertion (%d)!\n",
//              sol_index, removed->value);
        } else {
//          d_debug(
//              "Alternatives %d. Restoring previously removed value (%d)!\n",
//              sol_index, removed->value);
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
//			d_debug(
//					"Alternatives %d. Partial solution in (%d, %d) is better than last value (%d >= %d)\n",
//					sol_index, snode, cur_var, auxl1->value, solutions[*sols_size - 1]->value);

			if(find_duplicate(solutions, *sols_size, auxl1, -1)) {
//        d_debug(
//            "Alternatives %d. Partial solution in (%d, %d) is already present!\n",
//            sol_index, snode, cur_var);
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

//	d_dec_indent();
}

void backtracking(InnerSolution* solutions, InnerSolution sol_dest, size_t
    sol_index, size_t* sols_size, size_t K, int** matrix, KProblem problem, bool alternative)
{
	ssize_t tmp;
	size_t snode, var, last_var, limit_var;
	uint32_t value, cum_val;

  snode = sol_dest->row_idx;
  var = sol_dest->column_idx;
  value = sol_dest->value;

//	d_inc_indent();
//	d_debug("Backtracking %d (%d): [%d in (%d, %d)]\n", sol_index,
//			solutions[sol_index]->value, value, snode, var);

	cum_val = 0;
	// Starting from the solution supernode, removes the current variable
	// and add it to the solution vector
	while (1) {

		value -= problem->values[var];
		cum_val += problem->values[var];
		set_inner_sol_element(sol_dest, var, sol_dest->sol_vector[var] + 1);

		limit_var = sol_dest->column_idx;
		last_var = var;

		if (snode + 1 == problem->weights[var]) { // Backtrack terminated
//			d_debug("Backtracking %d (%d): terminated with var %d\n", sol_index,
//					solutions[sol_index]->value, var);
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

//    d_debug("Backtracking %d (%d): "
//            "Found [%d in (%d, %d)] by subtracting var %d "
//            "(w:%d, v:%d), cum_val: %d, limit: %d\n",
//        sol_index, solutions[sol_index]->value, value, snode, tmp,
//        var, problem->weights[var], problem->values[var], cum_val,
//        limit_var);

    var = (size_t) tmp;
    if(alternative)
      search_alternative_solutions(snode, var, cum_val, limit_var,
          sol_index, sol_dest, solutions, sols_size, K, matrix, problem,
          last_var);

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
//		d_debug("Recovering for solution %d [%d in (%d, %d)]\n", i,
//				solutions[i]->value, solutions[i]->row_idx,
//				solutions[i]->column_idx);

		if(i % steps == 0) {
      d_notice("Recovering for solution %d of %d\n", i + 1, *size);
    }

		if (!solutions[i]->recovered) {

			kp_copy_inn_sol(&auxl, solutions[i]);
//      auxl = solutions[i];
			backtracking(solutions, auxl, i, size, K, matrix, problem, true);
			kp_free_inn_sol(solutions[i]);
			solutions[i] = auxl;

		}
	}

}
