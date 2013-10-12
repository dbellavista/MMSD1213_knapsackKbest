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

void search_alternative_solutions(size_t snode, size_t cur_var, uint32_t
    cum_val, size_t limit_var, size_t sol_index, InnerSolution* solutions,
    size_t sols_size, size_t K, int** matrix, KProblem problem, size_t
    last_var)
{

	ssize_t tmp;
  ssize_t i;
	size_t insert_idx;
	size_t var;
  uint32_t newvalue;
	InnerSolution auxl1, removed;

	d_inc_indent();

  // From the current node, trying to find an alternative solution by adding
  // different (feasible) variables
  var = limit_var;
	do { // for(var = limit_var; var >= 0; var--) {
		d_debug(
				"Alternatives %d (%d): Start: (%d, %d). Limit: %d. Trying position (%d, %d) -> value: %d\n",
				sol_index, solutions[sol_index]->value, snode, cur_var,
				limit_var, snode, var, matrix[snode][var]);

		if (var == cur_var || matrix[snode][var] < 0) {
			d_debug(
					"Alternatives %d (%d): Start: (%d, %d). Skipping position (%d, %d)\n",
					sol_index, solutions[sol_index]->value, snode, cur_var,
					snode, var);
			continue;
		}

		newvalue = matrix[snode][var] + cum_val;
		if (newvalue < solutions[sols_size - 1]->value) {
			d_debug(
					"Alternatives %d (%d): Start: (%d, %d). Skipping position (%d, %d) (value %d ok but not worth).\n",
					sol_index, solutions[sol_index]->value, snode, cur_var,
					snode, var, newvalue);
			continue;
		}
		// Newvalue is worth to be backtracked!
		
		d_debug(
				"Alternatives %d (%d): new solution from position (%d, %d): %d by adding cum value %d to %d\n",
				sol_index, solutions[sol_index]->value, snode, var, newvalue,
				cum_val, matrix[snode][var]);

    // Find a suitable position for the solution insertion
		tmp = find_idx_and_prepare_insertion(solutions, &sols_size, &removed, sol_index,
				newvalue, K);
		if (tmp == -1) { // Already have K solutions better than the new value
			d_debug(
					"Alternatives %d (%d): new solution %d not inserted, value equals to last solution and solution vector full.\n",
					sol_index, solutions[sol_index]->value, newvalue);
			continue;
		}
		insert_idx = (size_t) tmp;

		kp_init_inn_sol(&solutions[insert_idx], problem->num_var,
				solutions[sol_index]->column_idx, solutions[sol_index]->row_idx,
				newvalue);

		kp_init_inn_sol(&auxl1, problem->num_var, var, snode,
				matrix[snode][var]);

		// Backtrack the new value in order to fill the solution vector
		backtracking(solutions, auxl1, insert_idx, sols_size, K, matrix,
				problem);

		solutions[insert_idx]->recovered = true;

		sum_solution_vectors(solutions[insert_idx], solutions[sol_index],
				auxl1);

		// TODO: why?
		if (matrix[snode][var] > (int) solutions[sols_size - 1]->value) {
			d_debug(
					"Alternatives %d: [(%d, %d)]. Var %d,  Value in matrix %d better\n",
					sol_index, snode, cur_var, matrix[snode][var]);

			tmp = find_idx_and_prepare_insertion(solutions, &sols_size, NULL,
					insert_idx, matrix[snode][var], K);
			if (tmp == -1) {
				d_error(
						"cannot find an idx for second insertion! Value: %d, SolIndex: %d\n",
						matrix[snode][var], insert_idx);
				continue;
			}
			insert_idx = (uint32_t) tmp;
			solutions[insert_idx] = auxl1;

		} else {
			kp_free_inn_sol(auxl1);
		}

	} while(var-- != 0);

	d_dec_indent();
}

void backtracking(InnerSolution* solutions, InnerSolution sol_dest, size_t
    sol_index, size_t sols_size, size_t K, int** matrix, KProblem problem)
{
	ssize_t tmp;
	size_t snode, var, last_var, limit_var;
	uint32_t value, cum_val;

  snode = sol_dest->row_idx;
  var = sol_dest->column_idx;
  value = sol_dest->value;

	d_inc_indent();
	d_debug("Backtracking %d (%d): [%d in (%d, %d)]\n", sol_index,
			solutions[sol_index]->value, value, snode, var);

	cum_val = 0;
	// Starting from the solution supernode, removes the current variable
	// and add it to the solution vector
	while (1) {
		if (snode < problem->weights[var]) { // Backtrack terminated
			d_debug("Backtracking %d (%d): terminated with var %d\n", sol_index,
					solutions[sol_index]->value, var);
			break;
		}

		snode -= problem->weights[var];
		value -= problem->values[var];
		cum_val += problem->values[var];
		sol_dest->sol_vector[var] += 1;
		limit_var = sol_dest->column_idx;
		last_var = var;

		// Finds the index of the value: it must be in the matrix
		tmp = find_idx(matrix[snode], var, (int) value);

		if (tmp < 0) { // Algorithm error
			d_debug("Backtracking %d (%d): NOT Found in snode %d, value %d\n",
					sol_index, solutions[sol_index]->value, snode, value);
			continue;
		}

    d_debug("Backtracking %d (%d): "
            "Found [%d in (%d, %d)] by subtracting var %d "
            "(w:%d, v:%d), cum_val: %d, limit: %d\n",
        sol_index, solutions[sol_index]->value, value, snode, tmp,
        var, problem->weights[var], problem->values[var], cum_val,
        limit_var);

    var = (size_t) tmp;
    search_alternative_solutions(snode, var, cum_val, limit_var,
        sol_index, solutions, sols_size, K, matrix, problem,
        last_var);

	}
	sol_dest->recovered = true;

	d_dec_indent();
}

void kp_recover_solution(InnerSolution* solutions, size_t size, size_t K,
    int** matrix, KProblem problem)
{
	InnerSolution auxl;
	uint32_t i;
	// For each solution, backtrack
	for (i = 0; i < size; i++) {
		d_debug("Recovering for solution %d [%d in (%d, %d)]\n", i,
				solutions[i]->value, solutions[i]->row_idx,
				solutions[i]->column_idx);
		if (!solutions[i]->recovered) {

			kp_copy_inn_sol(&auxl, solutions[i]);
			backtracking(solutions, auxl, i, size, K, matrix, problem);
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
