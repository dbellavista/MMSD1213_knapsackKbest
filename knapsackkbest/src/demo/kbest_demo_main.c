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
#include "kbest.h"

KBestSolutions single_test(char* filename, uint32_t K, double* test_ticks,
    double* test_time, bool do_prints);
void print_help();
void time_test(char* res_file, char** files, size_t files_num, uint32_t
    interaction, uint32_t K);
void print_array2(uint32_t* array, size_t size);
void check_solutions(KBestSolutions solutions);

void print_array2(uint32_t* array, size_t size)
{
	size_t i;
	printf("[ ");
	for (i = 0; i < size; i++) {
		printf("%u ", array[i]);
	}
	printf("]\n");
}

KBestSolutions single_test(char* filename, uint32_t K, double* test_ticks,
    double* test_time, bool do_prints)
{
	time_t start, stop;
	clock_t ticksStart, ticksStop;
	KProblem problem;
	KBestSolutions solutions;
	if(do_prints)
    d_notice("Reading problem %s\n", filename);
	read_problem(&problem, filename);
	if(do_prints) {
    print_kproblem(problem);
    d_notice("Solving problem with K=%lu\n", K);
  }

	time(&start);
	ticksStart = clock();
	kp_solve(&solutions, problem, K);
	ticksStop = clock();
	time(&stop);

	*test_ticks = ticksStop - ticksStart;
	*test_time = difftime(stop, start);
	kp_free_kp(problem);

	return solutions;
}

void time_test(char* res_file, char** files, size_t files_num, uint32_t
    iterations, uint32_t K)
{
	double test_ticks, test_time;
	size_t fileid;
	uint32_t i;
	FILE* f;

	f = fopen(res_file, "w");
  // Performing tests
	for (fileid = 0; fileid < files_num; fileid++) {
	  d_notice("Starting with file %s\n", files[fileid]);
    d_inc_indent();
    for(i = 0; i < iterations; i++) {

      printf("\r%d on %d", i, iterations);
      fflush(stdout);

      kp_free_kbest_sols(single_test(files[fileid], K, &test_ticks, &test_time, false));
      fprintf(f, "%f", test_ticks / CLOCKS_PER_SEC);
      if(i + 1 < iterations) {
        fprintf(f, ",");
      }
      fflush(f);
    }
    printf("\r");
    d_dec_indent();
    if(fileid +1 < files_num) {
      fprintf(f, "\n");
    }
	}
  fclose(f);
}

void check_solutions(KBestSolutions solutions)
{
	size_t i, j;
	size_t size;
	uint32_t acc;
	size = solutions->problem->num_var;

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

void print_help()
{
  d_warning(
      "Usage: (e|t|s) [args]\n"
      "\te <K>: example test. using ./problems/problem.txt\n"
      "\tt <k> <res_file> <num_iterations> {file}: timing test, running num_iteration iterations on the given files\n"
      "\ts <K> <file>: single test on the given file, prints the solution.\n"
      );
}

int main(int argc, char **argv)
{
  uint32_t num_iteration, K;
	double totalTick, totalTime;
  KBestSolutions solutions;
	//timeTest(2900, &averageTick, &averageTime, &totalTick, &totalTime);

	set_debug_level(WARNING ^ ERROR);

  if(argc < 2) {
    print_help();
    return 1;
  }

  switch(argv[1][0]) {
    case 'e':
      if(argc != 3) {
        d_error("Mandatory parameters missing\n");
        return 4;
      }
      K = strtoul(argv[2], NULL, 10);
      solutions = single_test("problems/problem.txt", K, &totalTick, &totalTime, true);
      print_kbest_solution_default_format(solutions);
      check_solutions(solutions);
      kp_free_kbest_sols(solutions);

      break;
    case 't':
      if(argc < 5) {
        d_error("Mandatory parameters missing\n");
        return 3;
      }
      K = strtoul(argv[2], NULL, 10);
      num_iteration = strtoul(argv[4], NULL, 10);
      time_test(argv[3], &(argv[5]), argc - 5, num_iteration, K);
      break;
    case 's':
      if(argc != 4) {
        d_error("Mandatory parameters missing\n");
        return 5;
      }
      K = strtoul(argv[2], NULL, 10);
      solutions = single_test(argv[3], K, &totalTick, &totalTime, false);
      print_kbest_solution_default_format(solutions);
      check_solutions(solutions);
      kp_free_kbest_sols(solutions);
      break;
    default:
      d_error("Command not recognized...\n");
      print_help();
      return 2;
  }
	return 0;
}
