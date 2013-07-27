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
	InnerSolution* solutions = (InnerSolution*) malloc(
			sizeof(InnerSolution) * K);
	InnerSolution* solutions1 = (InnerSolution*) malloc(
			sizeof(InnerSolution) * K);
	*ret = (InnerSolution*) malloc(sizeof(InnerSolution) * K);

	uint32 counter, i, j, i1, j1, P, P1;
	bool fim = false, moreleft = false;
	counter = 0;
	i = problem->max_weigth;

	while (i-- >= problem->weights[0]) {
		j = problem->num_var;
		while (j-- > 0) {
			if (matrix[i][j] >= 0) {
				kp_init_inn_sol(&solutions[counter], problem->num_var, j, i,
						matrix[i][j]);
				counter++;
				if (counter == K) {
					i1 = i;
					j1 = j;
					moreleft = true;
					i = -1;
					j = -1;
				}
			}
		}
	}
	P = counter;
	sort_by_values_non_inc(solutions, P);
	if (P == K && (i1 > problem->weights[0] || j1 > 1)) {
		fim = true;
	}
	while (fim) {
		counter = 0;
		i = i1 + 1;
		fim = false;
		while (i >= problem->weights[0]) {
			i = i - 1;
			j = problem->num_var;
			if (moreleft) {
				j = j1;
				moreleft = false;
			}
			while (j-- > 0) {
				if (matrix[i][j] > solutions[K - 1]->value) {
					kp_init_inn_sol(&solutions1[counter], problem->num_var, j,
							i, matrix[i][j]);
					counter++;
					if (counter == K) {
						i1 = i;
						j1 = j;
						moreleft = true;
						i = -1;
						j = -1;
					}
				}
			}
		}

		P1 = counter;
		sort_by_values_non_inc(solutions1, P1);
		if (solutions1[0]->value > solutions[K - 1]->value) {
			join_inner_solutions(*ret, solutions, solutions1, P, P1, K, true);
			solutions = *ret;
		}
		if (i1 > problem->weights[0] || j1 > 0) {
			fim = true;
		}
	}
	*ret_size = P;
	free(solutions);
	free(solutions1);
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
			if ((snode_idx + 1) + problem->weights[var] <= problem->max_weigth) {
				value = max(value, matrix[snode_idx][var]);
				matrix[snode_idx + problem->weights[var]][var] = value
						+ problem->values[var];
			}

		}
	}
}
