/*
 * main.c
 *
 *  Created on: Aug 5, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kbest_io_utility.h"
#include "kbest.h"
#include "get_real_time.h"

#ifndef MAX_PATH
#define MAX_PATH 4096
#endif


KBestSolutions single_test(char* filename, uint32_t K, double* test_time, bool
    do_prints);
void print_help();
void time_test(char* res_file, char** files, size_t files_num, uint32_t
    interaction, uint32_t K);
void project_test(char* base, char* K_list, uint32_t max_n);
void print_array2(uint32_t* array, size_t size);
void check_solutions(KBestSolutions solutions);
void interactive_test();

void print_array2(uint32_t* array, size_t size)
{
	size_t i;
	printf("[ ");
	for (i = 0; i < size; i++) {
		printf("%u ", array[i]);
	}
	printf("]\n");
}

KBestSolutions single_test(char* filename, uint32_t K, double* test_time, bool do_prints)
{
  double time_start, time_stop;
	KProblem problem;
	KBestSolutions solutions;
	if(do_prints)
    d_notice("Reading problem %s\n", filename);
	read_problem(&problem, filename);
	if(do_prints) {
    print_kproblem(problem);
    d_notice("Solving problem with K=%lu\n", K);
  }

	time_start = get_real_time();
	kp_solve(&solutions, problem, K);
	time_stop = get_real_time();

	*test_time = time_stop - time_start;

	kp_free_kp(problem);

	return solutions;
}

void interactive_test()
{
  char filename[MAX_PATH];
  char klist[MAX_PATH];
  filename[0] = 0;
  uint32_t K, maxN;
  FILE* f;
  bool ok_filename = false, ok_k = false;
  double test_time;
  bool selected_1 = false;
  KBestSolutions sol;

  printf("\n\n[*] Bellavista Daniele.\n"
             "[*] KBEST SOLVER , Interactive mode:\n\n");

  printf("[1] Solve a single problem\n");
  printf("[2] Solve a library type\n");
  printf("[-] The choice is yours: ");
  while(1) {
    fflush(stdout);
    if (fgets(filename, MAX_PATH - 1, stdin) == NULL) {
      printf("\n[#] Bye!\n");
      return;
    }
    if(filename[0] == '1') {
      selected_1 = true;
      break;
    } else if(filename[0] == '2') {
      selected_1 = false;
      break;
    }
  }
  if(selected_1) {
    while(!ok_filename) {
      printf("\t[+] Set the problem to solve: ");
      fflush(stdout);
      if (fgets(filename, MAX_PATH - 1, stdin) == NULL) {
        printf("\n[#] Bye!\n");
        return;
      }
      filename[MAX_PATH - 1] = 0;
      filename[strlen(filename) - 1] = 0;
      f = fopen(filename, "r");
      if(f != NULL) {
        ok_filename = true;
      } else {
        printf("\t[*] Sorry %s doesn't exists...\n", filename);
      }
    }
    while(!ok_k) {
      printf("\t[+] Set K: ");
      fflush(stdout);
      if(scanf(" %d", &K) != 1 || K == 0) {
        printf("[*] Sorry, K must be a positive integer\n");
      }
      ok_k = true;
    }
    printf("[+] Solving the solutions in b_solutions_single.out\n");
    sol = single_test(filename, K, &test_time, false);
    printf("[+] Solve time: %f s\n", test_time);
    f = fopen("b_solutions_single.out", "w");
    print_kbest_solution_default_format(f, sol);
    fclose(f);
  } else {
    printf("\t[+] Set the base directory of the instances (a path /path/nxx/): ");
    fflush(stdout);
    if (fgets(filename, MAX_PATH - 1, stdin) == NULL) {
      printf("\n[#] Bye!\n");
      return;
    }
    filename[MAX_PATH - 1] = 0;
    filename[strlen(filename) - 1] = 0;
    printf("\t[+] Set a K list (comma separated): ");
    fflush(stdout);
    if (fgets(klist, MAX_PATH - 1, stdin) == NULL) {
      printf("\n[#] Bye!\n");
      return;
    }
    while(1) {
      printf("\t[+] Set the maximum n present: ");
      fflush(stdout);
      if(scanf(" %d", &maxN) != 1 || maxN == 0) {
        printf("[*] Sorry, m must be a positive integer\n");
        continue;
      }
      break;
    }
    klist[MAX_PATH - 1] = 0;
    klist[strlen(filename) - 1] = 0;
    printf("[+] Going to solve... It might take a while\n");
    project_test(filename, klist, maxN);
  }
}

void project_test(char* base, char* K_list, uint32_t max_n)
{
	FILE *sol_f, *time_f;
	KBestSolutions solutions;
  double test_time, max_time, min_time, average_time;
	uint32_t files_num, curr_n;
	char path[MAX_PATH];
	FILE* f;
  char *token, *string, *tofree;
  uint32_t K;
  bool written_header;

  d_notice("Starting the project solve on %s\n", base);
  d_inc_indent();
  d_notice("Note that this will bruteforce the n*** folders!\n");
  d_dec_indent();

  for(curr_n = 1; curr_n <= max_n; curr_n++) {

    sprintf(path, "%s/n%u/i0/test.in", base, curr_n);
    f = fopen(path, "r");
    if(f == NULL) {
      continue;
    }
    fclose(f);
    d_notice("Found n%u! Staring to solve\n", curr_n);
    d_inc_indent();

    sprintf(path, "%s/n%u/b_computational_times.out", base, curr_n);
    time_f = fopen(path, "w");
    if(!time_f) {
      printf("Error opening file %s for writing...\n", path);
      exit(1);
    }

    written_header = false;
    tofree = string = strdup(K_list);
    while ((token = strsep(&string, ",")) != NULL) { // For each K
      K = strtoul(token, NULL, 10);
      if(K == 0) {
        continue;
      }
      d_notice("Solving for K = %u\n", K);
      files_num = 0;
      max_time = 0.0;
      min_time = INFINITY;
      average_time = 0.0;

      while(1) { // Solve each instance
        sprintf(path, "%s/n%u/i%u/test.in", base, curr_n, files_num);
        f = fopen(path, "r");
        if(f == NULL) {
          break;
        }
        fclose(f);

        solutions = single_test(path, K, &test_time, false);

        sprintf(path, "%s/n%u/i%u/b_k%u_solutions.out", base, curr_n, files_num, K);
        sol_f = fopen(path, "w");
        if(!sol_f) {
          printf("Error opening file %s for writing...\n", path);
          exit(1);
        }
        print_kbest_solution_default_format(sol_f, solutions);
        kp_free_kbest_sols(solutions);
        fclose(sol_f);

        if(test_time > max_time) {
          max_time = test_time;
        }
        if(test_time < min_time) {
          min_time = test_time;
        }
        average_time += test_time;
        files_num++;
      }
      average_time /= files_num;

      if(!written_header) {
        fprintf(time_f, "Computational times on %u instances:\n\n", files_num);
      }
      fprintf(time_f, "K = %u\n", K);
      fprintf(time_f, "average partial time = %f s\n", average_time);
      fprintf(time_f, "average total time = %f s\n", average_time);
      fprintf(time_f, "minimum total time = %f s\n", min_time);
      fprintf(time_f, "maximum total time = %f s\n", max_time);
      fprintf(time_f, "\n");

    }
    fclose(time_f);
    d_notice("Done, continuing bruteforcing...\n");
    free(tofree);
    d_dec_indent();
  }
}

void time_test(char* res_file, char** files, size_t files_num, uint32_t
    iterations, uint32_t K)
{
	double test_time;
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

      kp_free_kbest_sols(single_test(files[fileid], K, &test_time, false));
      fprintf(f, "%f", test_time);
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
      "Usage: [(e|t|s) [args]]\n"
      "\twith no parameters the interactive mode starts\n"
      "\te <K>: example test. using ./problems/problem.txt\n"
      "\tt <k> <res_file> <num_iterations> {file}: timing test, running num_iteration iterations on the given files\n"
      "\ts <K> <file>: single test on the given file, prints the solution.\n"
      "\tp <k_list> <base_dir> <max_n>: project test, k_list is a comma separated list of K, base_dir is dir parent of the n*** folders and max_n is and hint of the maximum *** value.\n"
      );
}

int main(int argc, char **argv)
{
  uint32_t num_iteration, K, max_n;
	double total_time;
  KBestSolutions solutions;
	//timeTest(2900, &averageTick, &averageTime, &totalTick, &totalTime);

	set_debug_level(NOTICE ^ WARNING ^ ERROR);

  if(argc < 2) {
    print_help();
    interactive_test();
    return 0;
  }

  switch(argv[1][0]) {
    case 'e':
      if(argc != 3) {
        d_error("Mandatory parameters missing\n");
        return 4;
      }
      K = strtoul(argv[2], NULL, 10);
      solutions = single_test("problems/problem.txt", K, &total_time, true);
      print_kbest_solution_default_format(NULL, solutions);
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
    case 'p':
      if(argc < 5) {
        d_error("Mandatory parameters missing\n");
        return 6;
      }
      max_n = strtoul(argv[4], NULL, 10);
      project_test(argv[3], argv[2], max_n);
      break;
    case 's':
      if(argc != 4) {
        d_error("Mandatory parameters missing\n");
        return 5;
      }
      K = strtoul(argv[2], NULL, 10);
      solutions = single_test(argv[3], K, &total_time, false);
      print_kbest_solution_default_format(NULL, solutions);
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
