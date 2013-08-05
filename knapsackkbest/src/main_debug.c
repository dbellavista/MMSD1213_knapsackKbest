/*
 * main.c
 *
 *  Created on: Aug 5, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include "../include/io_utility.h"
#include "../include/debug.h"
#include "../include/kbest.h"

int main(int argc, char **argv) {


	set_debug_level(WARNING ^ ERROR ^ NOTICE);

	KProblem problem;
	KBestSolutions solutions;
	read_problem(&problem, "test.in");
	print_kproblem(problem);

	kp_solve(&solutions, problem, 15);

	print_kbest_solution(solutions);


	kp_free_kp(problem);
	kp_free_kbest_sols(solutions);

	return 0;
}

