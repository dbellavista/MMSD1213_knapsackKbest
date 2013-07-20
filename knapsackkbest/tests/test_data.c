/*
 * testData.c
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdlib.h>
#include "kp_tests.h"
#include "../include/kp_alg/utility.h"
#define N 10

bool problems_equal(KProblem p1, KProblem p2);

char* results[] = { "failed", "successful" };
bool (*test_list[])(
		void) = {&test_matrix_alloc, &test_problem_creation, &test_solution_creation, &test_kbestsolutions_creation, &test_innersol_creation, &test_innersol_ordering, &test_innersol_join, NULL
};

uint16 weights[] = { 10, 4, 2, 7, 9, 2, 8, 37, 102, 1 };
uint16 values[N];

void set_up() {
	uint16 i;
	for (i = 0; i < N; i++) {
		values[i] = weights[i] * 10;
	}
}

void tear_down() {

}

void do_tests() {
	uint16 i = -1;
	bool res;
	while (test_list[++i] != NULL) {
		set_up();
		printf("==== Launching test no %d...\n", (i + 1));
		res = (*test_list[i])();
		printf("==== Test no %d %s\n\n", (i + 1), results[res]);
		tear_down();
	}
}

bool test_innersol_join() {
	printf("%s\n", __FUNCTION__);

	bool ret = true;
	uint16 j = 2, t = 3, count1 = 20, count2 = 20, K = 20;
	int k, i;

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
	for (; k >= (count1 + count2) - 2 * min(count1, count2) && k >= 0;
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
	printf("%s\n", __FUNCTION__);

	bool ret = true;
	uint16 i, j = 2, t = 3, count = 20;

	InnerSolution* ss = (InnerSolution*) malloc(count * sizeof(InnerSolution));
	for (i = 0; i < count; i++) {
		kp_init_inn_sol(&ss[i], N, j, t, i / 2);
	}
	sort_by_values_non_inc(ss, count);

	for (i = 0; i < count - 1; i++) {
		ret &= ss[i]->value >= ss[i + 1]->value;
	}

	for (i = 0; i < count; i++) {
		kp_free_inn_sol(ss[i]);
	}
	free(ss);
	return ret;
}

bool test_innersol_creation() {
	printf("%s\n", __FUNCTION__);

	bool ret = true;
	uint16 i, j = 2, t = 3, v = 10;

	InnerSolution s;
	kp_init_inn_sol(&s, N, j, t, v);

	ret &= s->dimension == N;
	ret &= s->column_idx == j;
	ret &= s->row_idx == t;
	ret &= s->value == v;

	for (i = 0; i < N; i++) {
		ret &= s->sol_vector[i] == 0;
	}
	for (i = 0; i < N; i++) {
		s->sol_vector[i] = i;
	}
	for (i = 0; i < N; i++) {
		ret &= s->sol_vector[i] == i;
	}

	kp_free_inn_sol(s);
	return ret;
}

bool test_matrix_alloc() {
	printf("%s\n", __FUNCTION__);

	uint16 i, j;
	uint16** matrix;
	uint16 w = 5, h = 5;
	allocate_matrix((void***) &matrix, w, h, sizeof(uint16));
	for (i = 0; i < w; i++) {
		for (j = 0; j < h; j++) {
			matrix[i][j] = (uint16) (i + (j << 4));
		}
	}
	for (i = 0; i < w; i++) {
		for (j = 0; j < h; j++) {
			if (!(matrix[i][j] == (uint16) (i + (j << 4)))) {
				return false;
			}
		}
	}
	free_matrix((void **) matrix);
	return true;
}

bool test_problem_creation() {
	printf("%s\n", __FUNCTION__);

	bool ret = true;
	uint16 i, mw = 50;

	KProblem p;
	kp_init_kp(&p, N, weights, values, mw);

	ret &= p->n == N;
	ret &= p->max_w == mw;

	for (i = 0; i < N - 1; i++) {
		ret &= p->weights[i] <= p->weights[i + 1];
		ret &= p->weights[i] * 10 == p->values[i];
	}

	kp_free_kp(p);
	return ret;
}

bool test_solution_creation() {
	printf("%s\n", __FUNCTION__);

	bool ret = true;

	KSolution s;
	kp_init_sol(&s, N);
	ret &= s->vector_size == N;
	uint16 i;

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
	printf("%s\n", __FUNCTION__);

	bool ret = true;
	uint16 sol_count = 10;
	KProblem p;
	kp_init_kp(&p, N, weights, values, 50);

	KBestSolutions s;
	kp_init_kbest_sols(&s, p, sol_count);

	ret &= problems_equal(p, s->problem);
	ret &= s->sol_count == sol_count;

	uint16 i, j;

	for (i = 0; i < s->sol_count; i++) {
		for (j = 0; j < s->problem->n; j++) {
			s->solutions[i]->solution_vector[j] = i + (j << 4);
		}
	}

	for (i = 0; i < s->sol_count; i++) {
		for (j = 0; j < s->problem->n; j++) {
			ret &= s->solutions[i]->vector_size == N;
			ret &= s->solutions[i]->solution_vector[j] == i + (j << 4);
		}
	}

	kp_free_kbest_sols(s);

	return ret;
}

bool problems_equal(KProblem p1, KProblem p2) {
	printf("%s\n", __FUNCTION__);

	uint16 i;
	if (p1->max_w != p2->max_w || p1->n != p2->n) {
		return false;
	}
	for (i = 0; i < p1->n; i++) {
		if (p1->weights[i] != p2->weights[i]
				|| p1->values[i] != p2->values[i]) {
			return false;
		}
	}
	return true;
}
