/*
 * kp_alg.c
 *
 *  Created on: Jul 18, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdlib.h>
#include "../include/kp_alg/kp_alg.h"
#include "../include/kp_alg/utility.h"

void search_alternative_solutions(uint16 row_idx, uint16 column_idx,
uint16 cumul_value, uint16 j1, uint16 index, InnerSolution* solutions,
uint16 sols_size, uint16 K, uint16** matrix, KProblem problem) {
	int tmp;
	uint16 k, g, f;
	uint16 s;
	InnerSolution auxl1;

	for (s = 0; s <= j1; s++) {
		if (s != column_idx) {
			if (matrix[row_idx][s] >= 0) {
				uint16 newvalue = matrix[row_idx][s] + cumul_value;
				if (newvalue >= solutions[sols_size - 1]->value) {
					tmp = find_idx_insertion(solutions, sols_size, index,
							newvalue);
					if (tmp == -1) {
						// TODO
						continue;
					}
					g = (uint16) tmp;
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
					kp_init_inn_sol(&solutions[g], problem->n,
							solutions[index]->column_idx,
							solutions[index]->row_idx, newvalue);

					kp_init_inn_sol(&auxl1, problem->n, s, row_idx,
							matrix[row_idx][s]);
					backtracking(solutions, auxl1, g, sols_size, K, matrix,
							problem);

					solutions[g].recovered = true;
					sum_solution_vectors(solutions[g], solutions[index], auxl1);
					if (matrix[row_idx][s] >= solutions[sols_size - 1].value) {
						tmp = find_idx_insertion(solutions, sols_size, g,
								matrix[row_idx][s]);
						if (tmp == -1) {
							// TODO
							continue;
						}
						k = (uint16) tmp;
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

void backtracking(InnerSolution* solutions, InnerSolution dest, uint16 index,
uint16 sols_size, uint16 K, uint16** matrix, KProblem problem) {
	uint16 t = dest->row_idx;
	uint16 j = dest->column_idx;
	uint16 z = dest->value;
	uint16 zcum = 0;
	uint16 j1;

	while (t >= 0) {
		t -= problem->weights[j];
		z -= problem->values[j];
		zcum += problem->values[j];
		dest->sol_vector[j] += 1;
		int tmp = find_idx(matrix[t], 0, j + 1, z);
		if (tmp < 0) {
			// TODO
		} else {
			j = (uint16) tmp;
		}
		j1 = dest->column_idx;
		if (t >= 0) {
			search_alternative_solutions(t, j, zcum, j1, index, solutions,
					sols_size, K, matrix, problem);
		}
	}
	dest->recovered = true;
}

void recover_solution(InnerSolution* solutions, uint16 size, uint16 K,
uint16** matrix, KProblem problem) {
	InnerSolution auxl;
	uint16 i = 0;
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

void kp_build_initial_best_k_list(InnerSolution** ret, uint16* ret_size,
uint16** matrix, KProblem problem, uint16 K) {
	InnerSolution* solutions = (InnerSolution*) malloc(
			sizeof(InnerSolution) * K);
	InnerSolution* solutions1 = (InnerSolution*) malloc(
			sizeof(InnerSolution) * K);
	*ret = (InnerSolution*) malloc(sizeof(InnerSolution) * K);

	uint16 counter, i, j, i1, j1, P, P1;
	bool fim = false, moreleft = false;
	counter = 0;
	i = problem->max_w;

	while (i-- >= problem->weights[0]) {
		j = problem->n;
		while (j-- > 0) {
			if (matrix[i][j] >= 0) {
				kp_init_inn_sol(&solutions[counter], problem->n, j, i,
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
			j = problem->n;
			if (moreleft) {
				j = j1;
				moreleft = false;
			}
			while (j-- > 0) {
				if (matrix[i][j] > solutions[K - 1]->value) {
					kp_init_inn_sol(&solutions1[counter], problem->n, j, i,
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

void kp_forward_enumeration(uint16** matrix, KProblem problem) {
	uint16 i, j, t, m, z;

	for (i = 0; i < problem->max_w; i++) {
		for (j = 0; j < problem->n; j++) {
			matrix[i][j] = -1;
		}
	}

// Begin forward enumeration
//// Begin initial ramification
	for (j = 0; j < problem->n; j++) {
		matrix[problem->weights[j]][j] = problem->values[j];
	}
////Begin ramification of supernodes
	for (t = problem->weights[0]; t <= problem->max_w - problem->weights[0];
			t++) {
		m = problem->n + 1;
		for (i = 0; i < problem->n; i++) {
			if (matrix[t][i] >= 0) {
				m = matrix[t][i];
				break;
			}
		}
		if (m != problem->n + 1) {
			z = matrix[t][m];
			for (i = m; i <= problem->n; i++) {
				if (t + problem->weights[i] <= problem->max_w) {
					if (matrix[t][i] > z) {
						z = matrix[t][i];
					}
					matrix[t + problem->weights[i]][i] = z + problem->values[i];
				}
			}
		}
	}
}
