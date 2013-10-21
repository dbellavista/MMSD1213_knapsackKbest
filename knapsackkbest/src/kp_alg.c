/*
 * kp_alg.c
 *
 *  Created on: Jul 18, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdlib.h>
#include "debug.h"
#include "kp_alg.h"
#include "utility.h"

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
