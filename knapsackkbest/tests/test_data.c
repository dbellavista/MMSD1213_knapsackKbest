/*
 * testData.c
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include "kp_tests.h"
#include "../include/kp_alg/utility.h"
#define N 10

bool problems_equal(KProblem p1, KProblem p2);

char* results[] = { "failed", "successful" };
bool (*test_list[])(
		void) = {&test_matrix_alloc, &test_problem_creation, &test_solution_creation, NULL
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

bool test_matrix_alloc() {
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
	return true;
}

bool test_problem_creation() {
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
	bool ret = true;
	KProblem p;
	kp_init_kp(&p, N, weights, values, 50);

	KSolution s;
	kp_init_sol(&s, p, 10);

	ret &= problems_equal(p, s->problem);

	uint16 i, j;

	for (i = 0; i < s->sol_count; i++) {
		for (j = 0; j < s->problem->n; j++) {
			s->solutions[i][j] = i + (j << 4);
		}
	}

	for (i = 0; i < s->sol_count; i++) {
		for (j = 0; j < s->problem->n; j++) {
			ret &= s->solutions[i][j] == i + (j << 4);
		}
	}

	kp_free_sol(s);

	return ret;
}

bool problems_equal(KProblem p1, KProblem p2) {
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
