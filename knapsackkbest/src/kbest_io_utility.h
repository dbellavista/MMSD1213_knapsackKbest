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

/// The debug print level
enum debug_level {
  /// Display notices
	NOTICE = 0x1,
	/// Display warnings
	WARNING = 0x2,
	/// Display errors
	ERROR = 0x4,
	/// Display debug messages
	DEBUG = 0x8
};

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
 *  @brief          Prints to stdout the KBestSolutions representation using
 *                  the default format by Baldacci, Galassi and Leardini.
 *
 *  @param[in]      fout The FILE to write to. If NULL, stdout will be chosen.
 *  @param[in]      solution The solution to print
 *
 */
void print_kbest_solution_default_format(FILE* fout, KBestSolutions solution);

/**
 *  @brief          Prints to stdout a KBestSolutions representation.
 *
 *  @param[in]      solution The solution to print
 *
 */
void print_kbest_solution(KBestSolutions solution);

/**
 *  @brief          Sets the global debug level
 *
 *  @param[in]   debug   A combination of debug_level
 */
void set_debug_level(unsigned int debug);

/**
 *  @brief          Prints a notice message
 *
 *  @param[in]   format   The format string
 *
 */
void d_notice(char* format, ...);
/**
 *  @brief          Prints a warning message
 *
 *  @param[in]   format   The format string
 *
 */
void d_warning(char* format, ...);
/**
 *  @brief          Prints an error message
 *
 *  @param[in]   format   The format string
 *
 */
void d_error(char* format, ...);
/**
 *  @brief          Prints a warning message
 *
 *  @param[in]   format   The format string
 *
 */
void d_debug(char* format, ...);

/**
 *  @brief          Increments a global indentation level
 *
 */
void d_inc_indent();
/**
 *  @brief          Decrements the global indentation level
 *
 */
void d_dec_indent();

#endif /* KBEST_IO_UTILITY_H_ */
