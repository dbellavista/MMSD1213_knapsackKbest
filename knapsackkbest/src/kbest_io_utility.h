/**
 *
 * @file    kbest_io_utility.h
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    10/21/2013 12:15:02 PM
 * @brief   IO utilities for importing problems from files and printing solutions to stdout.
 *
 */

#ifndef KBEST_IO_UTILITY_H_
#define KBEST_IO_UTILITY_H_

#include "kbest.h"

/**
 *  @brief          Read a problem from the given file.
 *
 *  @param[out]      dest  A pointer to a KProblem to be filled with the read problem.
 *  @param[in]       file  The file to read
 *
 *  @return         Returns true if the problem was correctly read, false otherwise.
 *
 *  @details        The file must be an ASCII file with:
 *                  @code
 *                  <Number_of_variables>
 *                  <Maximum_weight>
 *                  {<weight> <value>\n}^n
 *
 *                  @endcode
 *                  where n is the number of variables.
 *                  For instance:
 *
 *                  @code
 *                  5
 *                  20
 *                  5 1
 *                  4 2
 *                  3 3
 *                  2 4
 *                  1 5
 *                  @endcode
 *
 *                  Generates a problem with 5 variables and 20 as max weight.
 */
bool read_problem(KProblem* dest, char* file);

/**
 *  @brief          Prints to stdout a KProblem representation.
 *
 *  @param[in]      problem The problem to print
 *
 */
void print_kproblem(KProblem problem);

/**
 *  @brief          Prints to stdout a KBestSolutions representation.
 *
 *  @param[in]      solution The solution to print
 *
 */
void print_kbest_solution(KBestSolutions solution);


#endif /* KBEST_IO_UTILITY_H_ */
