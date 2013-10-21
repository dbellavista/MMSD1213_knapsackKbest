/*
 * main.c
 *
 *  Created on: Aug 5, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kbest_io_utility.h"
#include "utility/debug.h"
#include "kbest.h"

#define NO_TESTS 20

void print_array2(uint32_t* array, size_t size) {
	size_t i;
	printf("[ ");
	for (i = 0; i < size; i++) {
		printf("%u ", array[i]);
	}
	printf("]\n");
}

KBestSolutions singleTest(char* filename, uint32_t K, double* test_ticks,
		double* test_time) {
	time_t start, stop;
	clock_t ticksStart, ticksStop;
	KProblem problem;
	KBestSolutions solutions;
	read_problem(&problem, filename);

//  size_t i;
//	for(i = 0; i < problem->num_var; i++) {
//    problem->values[i] = problem->values[i] / 200 + 1;
//  }
//  problem->num_var = 2;
//  problem->max_weigth = 48;
	print_kproblem(problem);

	time(&start);
	ticksStart = clock();
	kp_solve(&solutions, problem, K);
	ticksStop = clock();
	time(&stop);

	*test_ticks = ticksStop - ticksStart;
	*test_time = difftime(stop, start);
	// print_kbest_solution(solutions);
	kp_free_kp(problem);

	return solutions;
}

void timeTest(uint32_t K, double* rAverageTick, double* rAverageTime,
		double* rTotalTick, double* rTotalTime) {

	double averageTick, averageTime, test_ticks, test_time, totalTime,
			totalTicks;
	char filename[100];
	uint32_t test;
	totalTicks = 0.0;
	totalTime = 0.0;
	for (test = 1; test <= NO_TESTS; test++) {

		sprintf(filename, "problems/test_%d.txt", test);

		free(singleTest(filename, K, &test_ticks, &test_time));

		totalTicks += test_ticks;
		totalTime += test_time;
	}

	averageTick = totalTicks / NO_TESTS;
	averageTime = totalTime / NO_TESTS;

	d_notice("Average: used %0.2f seconds of CPU time. \n",
			(double) averageTick / CLOCKS_PER_SEC);
	d_notice("Average: Finished in about %.0f seconds. \n", averageTime);

	d_notice("Total: used %0.2f seconds of CPU time. \n",
			(double) totalTicks / CLOCKS_PER_SEC);
	d_notice("Total: Finished in about %.0f seconds. \n", totalTime);

	*rAverageTick = averageTick;
	*rAverageTime = averageTime;
	*rTotalTick = totalTicks;
	*rTotalTime = totalTime;
}

void check_solutions(KBestSolutions solutions) {
	size_t i, j;
	size_t size = solutions->problem->num_var;
	uint32_t acc;

  d_notice("Checking consistency...\n");
  d_inc_indent();
  for (i = 0; i < solutions->sol_count; i++) {
    acc = 0;
    for(j = 0; j < size; j++) {
      acc += solutions->solutions[i]->solution_vector[j] * solutions->problem->values[j];
    }
    if(acc != solutions->solutions[i]->tot_value) {
      d_error("Inconsistency! Solution %zu has value %u, should be %u\n", i, solutions->solutions[i]->tot_value, acc);
    }
  }
  d_dec_indent();
  d_notice("Consistency done!\n\n");

  d_notice("Checking duplicates...\n");
  d_inc_indent();
	for (i = 0; i < solutions->sol_count; i++) {
		KSolution sol1 = solutions->solutions[i];
		for (j = i + 1; j < solutions->sol_count; j++) {
			KSolution sol2 = solutions->solutions[j];
			if (sol1->tot_value != sol2->tot_value) {
				break;
			}
			if (!memcmp(sol1->solution_vector, sol2->solution_vector, size * sizeof(uint32_t))) {
				d_error("Equals [%zu, %u] and [%zu, %u]\n", (i+1), sol1->tot_value, (j+1),
						sol2->tot_value);
				printf("\t\t"); print_array2(sol1->solution_vector, size);
        printf("\t\t"); print_array2(sol2->solution_vector, size);
			}
		}
	}
  d_dec_indent();
  d_notice("Duplicates done!\n\n");
}

int main(int argc, char **argv) {

	double totalTick, totalTime;
	//timeTest(2900, &averageTick, &averageTime, &totalTick, &totalTime);

	char* p1 = "problems/test_1.txt";
//	char* p1 = "problem.txt";

	char* filename;

	set_debug_level(WARNING ^ ERROR ^ NOTICE);
	if(argc > 1) {
		if(!strcmp(argv[1], "d")) {
			set_debug_level(WARNING ^ ERROR ^ NOTICE ^ DEBUG);
		}
	}

	if(argc > 2) {
		filename = argv[2];
	} else {
		filename = p1;
	}

	d_notice("Opening and solving %s\n", filename);

	KBestSolutions solutions = singleTest(filename, 2900, &totalTick, &totalTime);
	print_kbest_solution(solutions);

	check_solutions(solutions);
	kp_free_kbest_sols(solutions);

	return 0;
}

