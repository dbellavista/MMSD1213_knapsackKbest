/*
 * testData.c
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdlib.h>
#include "kp_tests.h"
#include "utility/utility.h"
#include "kbest_io_utility.h"
#include "kbest_io_utility.h"
#define N 10

bool problems_equal(KProblem p1, KProblem p2);
bool matrices_equal(int** m1, int** m2, uint32_t nrow, uint32_t ncols);

uint32_t ids[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
uint32_t weights[] = { 10, 4, 2, 7, 9, 2, 8, 37, 102, 1 };
uint32_t values[N];

KProblem test_problem;
#define TEST_N 5
#define TEST_MAXW 15
#define TEST_K 15
uint32_t test_ids[] = { 1, 2, 3, 4, 5 };
uint32_t test_weights[] = { 3, 4, 5, 6, 7 };
uint32_t test_values[] = { 4, 3, 5, 7, 8 };
int target_matrix_forward[TEST_MAXW][TEST_N] = { { -1, -1, -1, -1, -1 }, // 1
		{ -1, -1, -1, -1, -1 }, // 2
		{ 4, -1, -1, -1, -1 }, // 3
		{ -1, 3, -1, -1, -1 }, // 4
		{ -1, -1, 5, -1, -1 }, // 5
		{ 8, -1, -1, 7, -1 }, // 6
		{ -1, 7, -1, -1, 8 }, // 7
		{ -1, 6, 9, -1, -1 }, // 8
		{ 12, -1, 8, 11, -1 }, // 9
		{ -1, 11, 10, 10, 12 }, // 10
		{ -1, 10, 13, 12, 11 }, // 11
		{ 16, 9, 12, 15, 13 }, // 12
		{ -1, 15, 14, 14, 16 }, // 13
		{ -1, 14, 17, 16, 16 },  // 14
		{ 20, 13, 16, 19, 17 } }; // 15

uint32_t test_initial_sol[] = { 20, 19, 17, 17, 16, 16, 16, 16, 16, 15, 15, 14,
		14, 14, 13 };

uint32_t test_final_solutions[] = { 20, 19, 18, 17, 17, 16, 16, 16, 16, 16, 15,
		15, 15, 15, 15 };

void set_up() {
	uint32_t i;
	for (i = 0; i < N; i++) {
		values[i] = weights[i] * 10;
	}

	kp_init_kp(&test_problem, TEST_N, test_weights, test_values, test_ids, TEST_MAXW);
}

void tear_down() {
	kp_free_kp(test_problem);
}

void do_kp_tests() {
	set_debug_level(//DEBUG ^
	    NOTICE ^ ERROR ^ WARNING);

	tests("Data creation", &test_matrix_alloc, &test_problem_creation,
			&test_solution_creation, &test_kbestsolutions_creation,
			&test_innersol_creation, NULL);
	tests("Data utility", &test_innersol_ordering, &test_innersol_join,
			&test_find, &test_innersol_copy, &test_find_innsol_idx,
			&test_create_kbest_from_inner, NULL);
	tests("KP Algorithm", &test_kp_algorithm, &test_kp_solver, NULL);

	tests("Test input", &test_input_file, NULL);
}

bool test_input_file() {
	PRINT_INTRO("");

	bool ret = true;
	uint32_t i;

	KProblem problem;
	ret &= read_problem(&problem, "problems/test.txt");
	if(!problem) {
    return ret;
  }
	ret &= problem->max_weigth == 20;
	ret &= problem->num_var == 5;
	for (i = 0; i < 5; i++) {
		ret &= problem->weights[i] == i + 1 && problem->values[i] == 5 - i;
	}

	return ret;
}

bool test_kp_solver() {
	PRINT_INTRO("");

	bool ret = true;
	uint32_t i, v, j, k;
	KBestSolutions solutions;
	kp_solve(&solutions, test_problem, TEST_K);

	for (i = 0; i < solutions->sol_count; i++) {
		KSolution sol = solutions->solutions[i];
		ret &= sol->vector_size == TEST_N;
		// Check values correspondence
		ret &= test_final_solutions[i] == sol->tot_value;
		v = 0;
		// Check consistency
		for (j = 0; j < TEST_N; j++) {
			v += sol->solution_vector[j] * test_problem->values[j];
		}
		// Check unique.
		ret &= v == sol->tot_value;
		for (j = i + 1; j < solutions->sol_count; j++) {
			KSolution sol2 = solutions->solutions[j];
			if (sol2->tot_value == sol->tot_value) {
				k = 0;
				while (sol->solution_vector[k] == sol2->solution_vector[k]
						&& ++k < TEST_N)
					;
				if (k == TEST_N) {
					ret = false;
				}
			}
		}
	}

	kp_free_kbest_sols(solutions);

	return ret;
}

bool test_kp_algorithm() {
	PRINT_INTRO("");

	uint32_t i, j, k, v;
	bool ret, tmp;
	int** matrix;
  int** matrix2;
	size_t size;
	InnerSolution* listSol;

	ret = true;

	printf("Forward enumeration:\n");
	allocate_matrix((void ***) &matrix, test_problem->max_weigth,
			test_problem->num_var, sizeof(uint32_t));
	kp_forward_enumeration(matrix, test_problem);

	matrix2 = (int**) malloc(TEST_MAXW * sizeof(int *));
	for (i = 0; i < TEST_MAXW; i++) {
		matrix2[i] = target_matrix_forward[i];
	}
	ret = matrices_equal(matrix, matrix2, TEST_MAXW, TEST_N);
	printf("\tres: %d\n", ret);

	printf("Initial K solutions.\n");
	kp_build_initial_best_k_list(&listSol, &size, matrix, test_problem, TEST_K);
	tmp = size == TEST_K;
  if(!tmp) {
    printf("\tSize differs! Expected %u is %zu\n", TEST_K, size);
    ret = false;
  }
	for (i = 0; i < size; i++) {
		tmp = listSol[i]->value == test_initial_sol[i];
		if(!tmp) {
      printf("\tSolution %u expected %u is %u\n", i, test_initial_sol[i], listSol[i]->value);
      ret = false;
    }
	}
	printf("\tres: %d\n", ret);

	printf("Recover solutions.\n");
	kp_recover_solution(listSol, &size, TEST_K, matrix, test_problem);
	tmp = size == TEST_K;
	if(!tmp) {
    printf("\tNumber of solutions is %d, expected %d\n", tmp, TEST_K);
    ret = false;
  }
	printf("\tChecking consistency...\n");
	for (i = 0; i < size; i++) {
		// Check values correspondence
		tmp = test_final_solutions[i] == listSol[i]->value;
    if(!tmp) {
      printf("\t\tSol %d Different values expected: %u, got: %u\n", i, test_final_solutions[i], listSol[i]->value);
      ret = false;
    }
		v = 0;
		// Check consistency
		for (j = 0; j < TEST_N; j++) {
			v += listSol[i]->sol_vector[j] * test_problem->values[j];
		}
		tmp = v == listSol[i]->value;
    if(!tmp) {
      printf("\t\tSol %d inconsistent value (expected: %u, got: %u)\n", i, listSol[i]->value, v);
      ret = false;
    }
		// Check unique.
		for (j = i + 1; j < size; j++) {
			if (listSol[j]->value == listSol[i]->value) {
				k = 0;
				while (listSol[i]->sol_vector[k] == listSol[j]->sol_vector[k]
						&& ++k < TEST_N)
					;
				if (k == TEST_N) {
          printf("\t\tSolution %d and solution %d are equals\n", i, j);
					ret = false;
				}
			}
		}
	}

	// Cleaning up the mess
	for (i = 0; i < size; i++) {
		kp_free_inn_sol(listSol[i]);
	}
	free(listSol);
	free(matrix2);
	free_matrix((void**) matrix);

	return ret;
}

bool test_create_kbest_from_inner() {
	PRINT_INTRO("");
	bool ret = true;
	uint32_t size = 30, i, k;
	KProblem problem;
	kp_init_kp(&problem, N, weights, values, ids, 20);
	InnerSolution* ss = (InnerSolution*) malloc(size * sizeof(InnerSolution));

	for (i = 0; i < size; i++) {
		kp_init_inn_sol(&ss[i], N, 4, 4, i);
		for (k = 0; k < N; k++) {
			ss[i]->sol_vector[k] = i;
		}
	}
	sort_by_values_non_inc(ss, size);
	KBestSolutions solutions;
	create_kbest_solutions_from_inner(&solutions, ss, size, problem, true);
	ret &= solutions->sol_count == size;
	ret &= problems_equal(solutions->problem, problem);
	for (i = 0; i < size; i++) {
		ret &= solutions->solutions[i]->tot_value == size - 1 - i;
		ret &= solutions->solutions[i]->vector_size == N;
		for (k = 0; k < N; k++) {
			ret &= solutions->solutions[i]->solution_vector[k] == size - 1 - i;
		}
	}

	kp_free_kbest_sols(solutions);

	return ret;
}

bool test_find_innsol_idx() {
	PRINT_INTRO("");
	bool ret = true;
	size_t size = 30;
	size_t i, tmp;
	uint32_t value_to_insert;
	ssize_t g;

	InnerSolution* ss = (InnerSolution*) malloc(
			(size + 1) * sizeof(InnerSolution));

	for (i = 0; i < size; i++) {
		kp_init_inn_sol(&ss[i], N, 4, 4, i * 2 + 2);
	}
	sort_by_values_non_inc(ss, size);

  // [60 58 56 54 52 50 ... 4 2]
	value_to_insert = 9;
	g = find_idx_insertion(ss, size, size, -1, value_to_insert);
	ret &= ss[g]->value == 8;

	value_to_insert = 3;
	g = find_idx_insertion(ss, size, size, -1, value_to_insert);
	ret &= ss[g]->value == 2;

	value_to_insert = 0;
	g = find_idx_insertion(ss, size, size, -1, value_to_insert);
	ret &= g == -1;

	value_to_insert = 0;
	g = find_idx_insertion(ss, size, size + 1, -1, value_to_insert);
	ret &= (g >= 0) && (size_t) g == size;
	ret &= ss[g - 1]->value == 2;

	value_to_insert = 1000;
	g = find_idx_insertion(ss, size, size, -1, value_to_insert);
	ret &= g == 0;

	value_to_insert = 1000;
	g = find_idx_insertion(ss, size, size, 0, value_to_insert);
	ret &= g == -1;

	value_to_insert = 9;
	tmp = size;
	g = find_idx_and_prepare_insertion(ss, &tmp, 0, value_to_insert, size);
	ret &= size == tmp;
	ret &= ss[g] == NULL;
	ret &= ss[g + 1]->value == 8;
	ret &= ss[g - 1]->value == 10;
	ret &= ss[size - 1]->value == 4;
	ss[g] = ss[g + 1];

	value_to_insert = 9;
	tmp = size;
	g = find_idx_and_prepare_insertion(ss, &tmp, 0, value_to_insert, size + 1);
	ret &= tmp == size + 1;
	ret &= ss[g] == NULL;
	ret &= ss[g + 1]->value == 8;
	ret &= ss[g - 1]->value == 10;
	ret &= ss[size]->value == 4;

	for (i = 0; i < size + 1; i++) {
		if (ss[i] != NULL) {
			kp_free_inn_sol(ss[i]);
		} else {
			// Trick for ss[g] = ss[g + 1]
			i += 1;
		}
	}
	free(ss);

	return ret;
}

bool test_innersol_copy() {
	PRINT_INTRO("");
	bool ret = true;
	uint32_t i, j = 2, t = 3, v = 10;
	InnerSolution s, sc;
	kp_init_inn_sol(&s, N, j, t, v);
	for (i = 0; i < N; i++) {
		s->sol_vector[i] = i;
	}
	s->recovered = true;
	kp_copy_inn_sol(&sc, s);

	ret &= sc->column_idx == s->column_idx;
	ret &= sc->dimension == s->dimension;
	ret &= sc->recovered == s->recovered;
	ret &= sc->row_idx == s->row_idx;
	ret &= sc->value == s->value;

	for (i = 0; i < N; i++) {
		ret &= sc->sol_vector[i] == s->sol_vector[i];
	}

	kp_free_inn_sol(s);
	kp_free_inn_sol(sc);
	return ret;
}

bool test_find() {
	PRINT_INTRO("");
	bool ret = true;
	int value = 134;
	int idx = 4;
	int vector[] = { 10, 89, 12, 378, 291, 11, 29, 101, 22, 319 };

	vector[idx] = value;
	ret &= find_idx(vector, idx, value) == idx;
	ret &= find_idx(vector, idx - 1, value) == -1;

	return ret;
}

bool test_innersol_join() {
	PRINT_INTRO("");

	bool ret = true;
	uint32_t j = 2, t = 3, count1 = 20, count2 = 20, K = 20;
	int64_t i, k;

	InnerSolution* ss1 = (InnerSolution*) malloc(
			count1 * sizeof(InnerSolution));
	InnerSolution* ss2 = (InnerSolution*) malloc(
			count2 * sizeof(InnerSolution));

	for (i = 0; i < count1; i++) {
		kp_init_inn_sol(&ss1[i], N, j, t, i * 2);
	}
	sort_by_values_non_inc(ss1, count1);
	for (i = 0; i < count2; i++) {
		kp_init_inn_sol(&ss2[i], N, j, t, i * 2 + 1);
	}
	sort_by_values_non_inc(ss2, count2);

	InnerSolution* ss = (InnerSolution*) malloc(K * sizeof(InnerSolution));
	join_inner_solutions(ss, ss1, ss2, count1, count2, K, true);

	for (k = K - 1; k >= (count1 + count2) && k >= 0; k--) {
		ret &= ss[k] == NULL;
	}
	i = count1 + count2 - K;
	i = (i < 0) ? 0 : i;
	for (; k >= (count1 + count2) - 2 * MIN(count1, count2) && k >= 0;
			i++, k--) {
		ret &= ss[k]->value == i;
	}

	if (count1 < count2) {
		i++;
	}

	for (; k >= 0; k--, i += 2) {
		ret &= ss[k]->value == i;
	}

	for (i = 0; i < K; i++) {
		if (ss[i] != NULL) {
			kp_free_inn_sol(ss[i]);
		}
	}

	return ret;

}

bool test_innersol_ordering() {
	PRINT_INTRO("");

	bool ret = true;
	uint32_t i, count = 20;

	InnerSolution* ss = (InnerSolution*) malloc(count * sizeof(InnerSolution));
	for (i = 0; i < count; i++) {
		kp_init_inn_sol(&ss[i], i / 2, i / 2, i / 2, i / 2);
	}
	sort_by_values_non_inc(ss, count);

	for (i = 0; i < count - 1; i++) {
		ret &= ss[i]->value >= ss[i + 1]->value;
		uint32_t v = ss[i]->value;
		ret &= v == ss[i]->column_idx && v == ss[i]->dimension
				&& v == ss[i]->row_idx;
	}

	for (i = 0; i < count; i++) {
		kp_free_inn_sol(ss[i]);
	}
	free(ss);
	return ret;
}

bool test_innersol_creation() {
	PRINT_INTRO("");

	InnerSolution s;
	uint32_t i, j = 2, t = 3, v = 10;
	bool ret = true;

	kp_init_inn_sol(&s, N, j, t, v);

	ret &= s->dimension == N;
	ret &= s->column_idx == j;
	ret &= s->row_idx == t;
	ret &= s->value == v;

	for (i = 0; i < N; i++) {
		ret &= s->sol_vector[i] == 0 && !s->recovered;
	}

	for (i = 0; i < N; i++) {
		s->sol_vector[i] = i;
	}

	kp_free_inn_sol(s);
	return ret;
}

bool test_matrix_alloc() {
	PRINT_INTRO("");

	uint32_t i, j;
	uint32_t** matrix;
	uint32_t nr = 20, nc = 5;
	allocate_matrix((void***) &matrix, nr, nc, sizeof(uint32_t));
	for (i = 0; i < nr; i++) {
		for (j = 0; j < nc; j++) {
			matrix[i][j] = (uint32_t) (i + (j << 4));
		}
	}
	for (i = 0; i < nr; i++) {
		for (j = 0; j < nc; j++) {
			if (!(matrix[i][j] == (uint32_t) (i + (j << 4)))) {
				return false;
			}
		}
	}
	free_matrix((void **) matrix);
	return true;
}

bool test_problem_creation() {
	PRINT_INTRO("");

	bool ret = true;
	uint32_t i, mw = 50;

	KProblem p;
	kp_init_kp(&p, N, weights, values, ids, mw);

	ret &= p->num_var == N;
	ret &= p->max_weigth == mw;

	for (i = 0; i < N - 1; i++) {
		ret &= p->weights[i] <= p->weights[i + 1];
		ret &= p->weights[i] * 10 == p->values[i];
	}

	kp_free_kp(p);
	return ret;
}

bool test_solution_creation() {
	PRINT_INTRO("");

	bool ret = true;

	KSolution s;
	kp_init_sol(&s, N);
	ret &= s->vector_size == N;
	uint32_t i;

	for (i = 0; i < s->vector_size; i++) {
		s->solution_vector[i] = i;
	}

	for (i = 0; i < s->vector_size; i++) {
		ret &= s->solution_vector[i] == i;
	}

	kp_free_sol(s);

	return ret;
}

bool test_kbestsolutions_creation() {
	PRINT_INTRO("");

	bool ret = true;
	uint32_t sol_count = 10;
	KProblem p;
	kp_init_kp(&p, N, weights, values, ids, 50);

	KBestSolutions s;
	kp_init_kbest_sols(&s, p, sol_count);

	ret &= problems_equal(p, s->problem);
	ret &= s->sol_count == sol_count;

	uint32_t i, j;

	for (i = 0; i < s->sol_count; i++) {
		for (j = 0; j < s->problem->num_var; j++) {
			s->solutions[i]->solution_vector[j] = i + (j << 4);
		}
	}

	for (i = 0; i < s->sol_count; i++) {
		for (j = 0; j < s->problem->num_var; j++) {
			ret &= s->solutions[i]->vector_size == N;
			ret &= s->solutions[i]->solution_vector[j] == i + (j << 4);
		}
	}

	kp_free_kbest_sols(s);

	return ret;
}

bool problems_equal(KProblem p1, KProblem p2) {

	uint32_t i;
	if (p1->max_weigth != p2->max_weigth || p1->num_var != p2->num_var) {
		return false;
	}
	for (i = 0; i < p1->num_var; i++) {
		if (p1->weights[i] != p2->weights[i]
				|| p1->values[i] != p2->values[i]) {
			return false;
		}
	}
	return true;
}

bool matrices_equal(int** m1, int** m2, uint32_t nrow, uint32_t ncols) {
	uint32_t i, j, k;
	bool ret = true;

	for (i = 0; i < nrow; i++) {
		for (j = 0; j < ncols; j++) {
			if (m1[i][j] != m2[i][j]) {
				ret = false;
				printf("R%d: ", i);
				for (k = 0; k < ncols; k++) {
					printf("%d ", m1[i][k]);
				}
				printf("| ");
				for (k = 0; k < ncols; k++) {
					printf("%d ", m2[i][k]);
				}
				printf("\n");
				break;
			}
		}
	}

	return ret;
}
