/*
 * kp_alg.c
 *
 *  Created on: Jul 18, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdlib.h>
#include "../include/kp_alg/kp_alg.h"
#include "../include/kp_alg/utility.h"

void search_alternative_solutions(uint32 row_idx, uint32 column_idx,
uint32 cumul_value, uint32 j1, uint32 index, InnerSolution* solutions,
uint32 sols_size, uint32 K, int** matrix, KProblem problem) {
	int tmp;
	uint32 k, g, f;
	uint32 s;
	InnerSolution auxl1;

	for (s = 0; s <= j1; s++) {
		if (s != column_idx) {
			if (matrix[row_idx][s] >= 0) {
				uint32 newvalue = matrix[row_idx][s] + cumul_value;
				if (newvalue >= solutions[sols_size - 1]->value) {
					tmp = find_idx_insertion(solutions, sols_size, index,
							newvalue);
					if (tmp == -1) {
						// TODO
						continue;
					}
					g = (uint32) tmp;
					if (sols_size == K) {
						kp_free_inn_sol(solutions[K - 1]);
					} else {
						sols_size += 1;
					}
					f = sols_size - 1;

					while (f > g) {
						solutions[f] = solutions[f - 1];
						f--;
					}
					kp_init_inn_sol(&solutions[g], problem->num_var,
							solutions[index]->column_idx,
							solutions[index]->row_idx, newvalue);

					kp_init_inn_sol(&auxl1, problem->num_var, s, row_idx,
							matrix[row_idx][s]);
					backtracking(solutions, auxl1, g, sols_size, K, matrix,
							problem);

					solutions[g]->recovered = true;
					sum_solution_vectors(solutions[g], solutions[index], auxl1);
					if (matrix[row_idx][s] >= solutions[sols_size - 1]->value) {
						tmp = find_idx_insertion(solutions, sols_size, g,
								matrix[row_idx][s]);
						if (tmp == -1) {
							// TODO
							continue;
						}
						k = (uint32) tmp;
						if (sols_size == K) {
							kp_free_inn_sol(solutions[K - 1]);
						} else {
							sols_size += 1;
						}
						f = sols_size - 1;

						while (f > k) {
							solutions[f] = solutions[f - 1];
							f--;
						}
						solutions[k] = auxl1;
					}
				}
			}
		}
	}

}

void backtracking(InnerSolution* solutions, InnerSolution dest, uint32 index,
uint32 sols_size, uint32 K, int** matrix, KProblem problem) {
	uint32 t = dest->row_idx;
	uint32 j = dest->column_idx;
	uint32 z = dest->value;
	uint32 zcum = 0;
	uint32 j1;

	while (t >= 0) {
		t -= problem->weights[j];
		z -= problem->values[j];
		zcum += problem->values[j];
		dest->sol_vector[j] += 1;
		int tmp = find_idx(matrix[t], 0, j + 1, z);
		if (tmp < 0) {
			// TODO
		} else {
			j = (uint32) tmp;
		}
		j1 = dest->column_idx;
		if (t >= 0) {
			search_alternative_solutions(t, j, zcum, j1, index, solutions,
					sols_size, K, matrix, problem);
		}
	}
	dest->recovered = true;
}

void recover_solution(InnerSolution* solutions, uint32 size, uint32 K,
		int** matrix, KProblem problem) {
	InnerSolution auxl;
	uint32 i = 0;
	while (i < size) {
		if (!solutions[i]->recovered) {
			kp_copy_inn_sol(&auxl, solutions[i]);
			backtracking(solutions, auxl, i, size, K, matrix, problem);

			kp_free_inn_sol(solutions[i]);
			solutions[i] = auxl;
		}
		i++;
	}

}

void kp_build_initial_best_k_list(InnerSolution** ret, uint32* ret_size,
		int** matrix, KProblem problem, uint32 K) {

	InnerSolution** buffer_sol;
	allocate_matrix((void***) &buffer_sol, 3, K, sizeof(InnerSolution));

	InnerSolution* solutions = buffer_sol[0];
	InnerSolution* solutions1 = buffer_sol[1];
	InnerSolution* tmp_solutions = buffer_sol[2];

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
					snode = 0;
					var = 0;
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
						snode = 0;
						var = 0;
					}
				}
			}
		}

		P1 = counter;
		sort_by_values_non_inc(solutions1, P1);
		if (solutions1[0]->value > solutions[K - 1]->value) {
			join_inner_solutions(tmp_solutions, solutions, solutions1, P, P1, K,
			true);
			solutions = tmp_solutions;
		}
		if (P1 == K && (last_snode > problem->weights[0] - 1 || last_var > 0)) {
			fim = true;
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
