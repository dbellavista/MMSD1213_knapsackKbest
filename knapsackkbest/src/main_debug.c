/*
 * main.c
 *
 *  Created on: Aug 5, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include "../include/io_utility.h"
#include "../include/debug.h"
#include "../include/kbest.h"
#include <time.h>

#define NO_TESTS 20

int main(int argc, char **argv) {
	double averageTick, averageTime;

	time_t start, stop, allStart, allStop;
	clock_t ticksStart, ticksStop, allTStart, allTStop;

	set_debug_level(WARNING ^ ERROR ^ NOTICE);

	KProblem problem;
	KBestSolutions solutions;

	uint32 test;

	time(&allStart);
	allTStart = clock();
	char filename[100];
	for (test = 1; test <= NO_TESTS; test++) {

		sprintf(filename, "problems/test_%d.txt", test);

		read_problem(&problem, filename);
		//print_kproblem(problem);

		time(&start);
		ticksStart = clock();
		kp_solve(&solutions, problem, 2900);
		ticksStop = clock();
		time(&stop);

		averageTick += ticksStop - ticksStart;
		averageTime += difftime(stop, start);
		// print_kbest_solution(solutions);
	}
	time(&allStop);
	allTStop = clock();

	averageTick /= NO_TESTS;
	averageTime /= NO_TESTS;

	d_notice("Average: used %0.2f seconds of CPU time. \n",
			(double) averageTick / CLOCKS_PER_SEC);
	d_notice("Average: Finished in about %.0f seconds. \n", averageTime);

	d_notice("Total: used %0.2f seconds of CPU time. \n",
			(double) (allTStop - allTStart) / CLOCKS_PER_SEC);
	d_notice("Total: Finished in about %.0f seconds. \n",
			difftime(allStop, allStart));

	kp_free_kp(problem);
	kp_free_kbest_sols(solutions);

	return 0;
}

