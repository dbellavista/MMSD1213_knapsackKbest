/*
 * kp_alg.c
 *
 *  Created on: Jul 18, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdlib.h>
#include "../include/debug.h"
#include "../include/kp_alg/kp_alg.h"
#include "../include/kp_alg/utility.h"

void search_alternative_solutions(uint32 snode, uint32 cur_var,
uint32 cum_val, uint32 limit_var, uint32 sol_index, InnerSolution* solutions,
uint32 sols_size, uint32 K, int** matrix, KProblem problem, uint32 last_var) {
	d_inc_indent();

	int tmp;
	uint32 insert_idx;
	int64 var;
	InnerSolution auxl1;

	for (var = limit_var; var >= 0; var--) {
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
		uint32 newvalue = matrix[snode][var] + cum_val;
		if (newvalue < solutions[sols_size - 1]->value) {
			d_debug(
					"Alternatives %d (%d): Start: (%d, %d). Skipping position (%d, %d) (value %d ok but not worth).\n",
					sol_index, solutions[sol_index]->value, snode, cur_var,
					snode, var, newvalue);
			continue;
		}
		// Newvalue is worth to be backtracked!
		// TODO Mine: checking if the step before exists in the matrix => already seen
		if (find_idx(matrix[snode + problem->weights[last_var]], limit_var,
				matrix[snode][var] + problem->values[last_var]) >= 0) {
			// Solution already examined :) (maybe)
			d_debug(
					"Alternatives %d (%d): solution from position (%d, %d) %d already examined\n",
					sol_index, solutions[sol_index]->value, snode, var,
					newvalue);
			continue;
		}
		d_debug(
				"Alternatives %d (%d): new solution from position (%d, %d): %d by adding cum value %d to %d\n",
				sol_index, solutions[sol_index]->value, snode, var, newvalue,
				cum_val, matrix[snode][var]);

		tmp = find_idx_and_prepare_insertion(solutions, &sols_size, sol_index,
				newvalue, K);
		if (tmp == -1) {
			d_debug(
					"Alternatives %d (%d): new solution %d not inserted, value equals to last solution and solution vector full.\n",
					sol_index, solutions[sol_index]->value, newvalue);
			continue;
		}
		insert_idx = (uint32) tmp;

		kp_init_inn_sol(&solutions[insert_idx], problem->num_var,
				solutions[sol_index]->column_idx, solutions[sol_index]->row_idx,
				newvalue);

		kp_init_inn_sol(&auxl1, problem->num_var, var, snode,
				matrix[snode][var]);
		backtracking(solutions, auxl1, insert_idx, sols_size, K, matrix,
				problem);

		solutions[insert_idx]->recovered = true;

		sum_solution_vectors(solutions[insert_idx], solutions[sol_index],
				auxl1);

		if (matrix[snode][var] > (int) solutions[sols_size - 1]->value) {
			d_debug(
					"Alternatives %d: [(%d, %d)]. Var %d,  Value in matrix %d better\n",
					sol_index, snode, cur_var, matrix[snode][var]);

			tmp = find_idx_and_prepare_insertion(solutions, &sols_size,
					insert_idx, matrix[snode][var], K);
			if (tmp == -1) {
				d_error(
						"cannot find an idx for second insertion! Value: %d, SolIndex: %d\n",
						matrix[snode][var], insert_idx);
				continue;
			}
			insert_idx = (uint32) tmp;
			solutions[insert_idx] = auxl1;

		} else {
			kp_free_inn_sol(auxl1);
		}

	}
	d_dec_indent();
}

void backtracking(InnerSolution* solutions, InnerSolution sol_dest,
uint32 sol_index,
uint32 sols_size, uint32 K, int** matrix, KProblem problem) {

	d_inc_indent();

	uint32 value = sol_dest->value;
	uint32 cum_val = 0;
	uint32 limit_var;

	int64 snode = sol_dest->row_idx;
	uint32 var = sol_dest->column_idx;
	uint32 last_var;
	d_debug("Backtracking %d (%d): [%d in (%d, %d)]\n", sol_index,
			solutions[sol_index]->value, value, snode, var);
	while (snode >= 0) {
		snode -= problem->weights[var];
		value -= problem->values[var];
		cum_val += problem->values[var];
		sol_dest->sol_vector[var] += 1;
		limit_var = sol_dest->column_idx;
		last_var = var;
		if (snode < 0) {
			d_debug("Backtracking %d (%d): terminated with var %d\n", sol_index,
					solutions[sol_index]->value, var);
			break;
		}
		int tmp = find_idx(matrix[snode], var, (int) value);
		if (tmp < 0) {
			d_debug("Backtracking %d (%d): NOT Found in snode %d, value %d\n",
					sol_index, solutions[sol_index]->value, snode, value);
			continue;
		} else {
			d_debug(
					"Backtracking %d (%d): Found [%d in (%d, %d)] by subtracting var %d (w:%d, v:%d), cum_val: %d, limit: %d\n",
					sol_index, solutions[sol_index]->value, value, snode, tmp,
					var, problem->weights[var], problem->values[var], cum_val,
					limit_var);
			var = (uint32) tmp;
			search_alternative_solutions(snode, var, cum_val, limit_var,
					sol_index, solutions, sols_size, K, matrix, problem,
					last_var);

		}
	}
	sol_dest->recovered = true;

	d_dec_indent();
}

void kp_recover_solution(InnerSolution* solutions, uint32 size,
uint32 K, int** matrix, KProblem problem) {
	InnerSolution auxl;
	uint32 i;
	for (i = 0; i < size; i++) {
		d_debug("Recovering for solution %d [%d in (%d, %d)]\n", i,
				solutions[i]->value, solutions[i]->row_idx,
				solutions[i]->column_idx);
		if (!solutions[i]->recovered) {
			auxl = solutions[i];
			backtracking(solutions, auxl, i, size, K, matrix, problem);

			// kp_free_inn_sol(solutions[i]);
			// solutions[i] = auxl;
		}
	}

}

void kp_build_initial_best_k_list(InnerSolution** ret, uint32* ret_size,
		int** matrix, KProblem problem, uint32 K) {

	InnerSolution** buffer_sol;
	allocate_matrix((void***) &buffer_sol, 3, K, sizeof(InnerSolution));

	InnerSolution* solutions = buffer_sol[0];
	InnerSolution* solutions1 = buffer_sol[1];
	InnerSolution* tmp_solutions = buffer_sol[2];

	InnerSolution* pIS;

	*ret = (InnerSolution*) malloc(sizeof(InnerSolution) * K);

	uint32 counter, P, P1;
	int snode, var, last_snode, last_var;
	bool fim = false, moreleft = false;
	counter = 0;
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
	while (fim) {
		counter = 0;
		fim = false;
		for (snode = last_snode; snode >= (int) problem->weights[0] - 1;
				snode--) {
			if (moreleft) {
				var = last_var - 1;
				moreleft = false;
			} else {
				var = problem->num_var - 1;
			}
			for (; var >= 0; var--) {
				// New value is better than last solution found
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
					&& (last_snode > problem->weights[0] - 1 || last_var > 0)) {
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

void kp_forward_enumeration(int** matrix, KProblem problem) {
	uint32 var, snode_idx, var_idx;
	int value;

	for (snode_idx = 0; snode_idx < problem->max_weigth; snode_idx++) {
		for (var = 0; var < problem->num_var; var++) {
			matrix[snode_idx][var] = -1;
		}
	}

	// Begin initial ramification
	//// Initial known solutions
	for (var = 0; var < problem->num_var; var++) {
		if(problem->weights[var] > problem->max_weigth) {
			continue;
		}
		matrix[problem->weights[var] - 1][var] = problem->values[var];
	}

	////Begin ramification of supernodes
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
		if (var_idx == problem->num_var) {
			continue;
		}
		// Update values
		value = matrix[snode_idx][var_idx];
		for (var = var_idx; var < problem->num_var; var++) {
			// weight = (snode_idx + 1)
			if ((snode_idx + 1) + problem->weights[var]
					<= problem->max_weigth) {
				value = max(value, matrix[snode_idx][var]);
				matrix[snode_idx + problem->weights[var]][var] = value
						+ problem->values[var];
			}

		}
	}
}
