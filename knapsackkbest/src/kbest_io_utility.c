/**
 *
 * @file    kbest_io_utility.c
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    10/21/2013 12:15:02 PM
 * @brief   IO utilities for importing problems from files and printing solutions to stdout.
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include "kbest_io_utility.h"
#include "utility/utility.h"

/// Close the FILE X and return Y
#define exit(X, Y) fclose(X); return Y;
/// Checks if X is equals to Y. If not, exit with status code Z
#define check(X, Y, Z) if (X != Y) { d_error("Error reading from file\n"); exit(Z, false); }

/// Current debug level
static unsigned int debug_level = NOTICE ^ WARNING ^ ERROR ^ DEBUG;
/// Global indentation level
static int indent = 0;

/**
 *  @brief          Print the \ref indent indentations
 *
 *  @param[in]   stream   The stream where to print
 */
void printIndent(FILE* stream);

/**
 *  @brief          Prints an array to stdout
 *
 *  @param[in]      array The array to print
 *  @param[in]     size The array size
 *
 */
void print_array(uint32_t* array, size_t size);

void print_array(uint32_t* array, size_t size) {
	uint32_t i;
	printf("[ ");
	for (i = 0; i < size; i++) {
		printf("%u ", array[i]);
	}
	printf("]\n");
}

void printIndent(FILE* stream) {
	int i;
	for(i = 0; i < indent; i++) {
		fprintf(stream, "\t");
	}
}

void print_kproblem(KProblem problem) {
	uint32_t size = problem->num_var;
	printf("Problem with %zu variables, max weight: %d\n", problem->num_var, problem->max_weigth);
	printf("\tValues ");
	print_array(problem->values, size);
	printf("\tWeights ");
	print_array(problem->weights, size);
}

void print_kbest_solution(KBestSolutions solution) {
	uint32_t size = solution->problem->num_var;
	printf("Solutions:\n");
	size_t i;
	for (i = 0; i < solution->sol_count; i++) {

		printf("\t%zu)\t%u\t-> ", i + 1, solution->solutions[i]->tot_value);
		print_array(solution->solutions[i]->solution_vector, size);
	}
	printf("\n");
}

bool read_problem(KProblem* dest, char* file) {

	*dest = NULL;
	FILE* fp = fopen(file, "r");
	if (fp == NULL) {
		d_error("Error opening file %s: %s\n", file, strerror(errno));
		return false;
	}

	/*
	 * Format:
	 * <Num Var>
	 * <Max W>
	 * <V1> <W1>
	 * <V2> <W2>
	 * ...
	 * <Vn> <Wn>
	 */
	uint32_t N, W, i;
	uint32_t *weights, *values;

	check(fscanf(fp, "%u", &N), 1, fp);
	d_debug("Read N = %d\n", N);

	check(fscanf(fp, "%u", &W), 1, fp);
	d_debug("Read W = %d\n", W);

	weights = (uint32_t*) malloc(N * sizeof(uint32_t));
	values = (uint32_t*) malloc(N * sizeof(uint32_t));

	for (i = 0; i < N; i++) {
		check(fscanf(fp, "%u %u", &values[i], &weights[i]), 2, fp);
		d_debug("Read var V%d = %d; W%d = %d\n", (i + 1), values[i], (i + 1),
				weights[i]);
	}

	d_debug("Problem read\n\n");
	kp_init_kp(dest, N, weights, values, W);

	free(weights);
	free(values);

	exit(fp, true);
}

void set_debug_level(unsigned int debug) {
	debug_level = debug;
}

void d_notice(char* format, ...) {
	if (debug_level & NOTICE) {
		va_list argptr;
		va_start(argptr, format);
		printIndent(stdout);
		fprintf(stdout, "- NOTICE: ");
		vfprintf(stdout, format, argptr);
		va_end(argptr);
	}
}

void d_warning(char* format, ...) {
	if (debug_level & WARNING) {
		va_list argptr;
		va_start(argptr, format);
		printIndent(stdout);
		fprintf(stdout, "+ WARNING: ");
		vfprintf(stdout, format, argptr);
		va_end(argptr);
	}
}

void d_error(char* format, ...) {
	if (debug_level & ERROR) {
		va_list argptr;
		va_start(argptr, format);
		printIndent(stdout);
		fprintf(stdout, "* ERROR: ");
		vfprintf(stdout, format, argptr);
		va_end(argptr);
	}
}

void d_debug(char* format, ...) {
	if (debug_level & DEBUG) {
		va_list argptr;
		va_start(argptr, format);
		printIndent(stdout);
		fprintf(stdout, "^ DEBUG: ");
		vfprintf(stdout, format, argptr);
		va_end(argptr);
	}
}

void d_inc_indent() {
	indent++;
}
void d_dec_indent() {
	if (indent) {
		indent--;
	}
}
