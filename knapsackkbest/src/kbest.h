/**
 *
 * @file    kbest.h
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    10/21/2013 11:45:28 AM
 * @brief   kbest.h is the library interface for solving K-Best Knapsack problems.
 *
 * @details
 *
 */

#ifndef KBEST_H_
#define KBEST_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/// Representation of a kbest problem
struct kProblem {
  /// Number of variables present
	size_t num_var;
	/// Array containing the weigths of the variables
	uint32_t* weights;
	/// Array containing the values of the variables
	uint32_t* values;
	/// The max weight for the problem
	uint32_t max_weigth;
};
/// A pointer to \ref kProblem
typedef struct kProblem* KProblem;

/// Representation of a solution for the kbest problem
struct kSolution {
  /// The solution value
	uint32_t tot_value;
	/// The size of \ref solution_vector
	size_t vector_size;
	/// The solution vector, containing the number of element for each variable
	uint32_t* solution_vector;
};
/// A pointer to \ref kSolution
typedef struct kSolution* KSolution;

/// The final solution to \ref kProblem, containing all the solutions found
struct kBestSolutions {
  /// The original kbest problem
	KProblem problem;
	/// The number of solution found
	size_t sol_count;
	/// Array containing the solutions
	KSolution* solutions;
};
/// A pointer to \ref kBestSolutions
typedef struct kBestSolutions* KBestSolutions;

/**
 *  @brief          Initialize a knapsack probelm
 *
 *  @param[out]     problem A pointer to filled with the new problem address
 *  @param[in]      n       Thee number of variables
 *  @param[in]      weights  An array of \p n elements containing the variables weight. Elements are copied into a new array.
 *  @param[in]      values  An array of \p n elements containing the variables values. Elements are copied into a new array.
 *  @param[in]      maxW    The maximum weight allowed
 *
 */
void kp_init_kp(KProblem* problem, size_t n, uint32_t* weights, uint32_t*
    values, uint32_t maxW);

/**
 *  @brief          Free an allocated \ref KProblem
 *
 *  @param[in]      problem   The problem to free
 */
void kp_free_kp(KProblem problem);

/**
 *  @brief          Initialize a KSolution
 *
 *  @param[out]     solution A pointer to filled with the new ksolution address
 *  @param[in]      n The number of variables
 */
void kp_init_sol(KSolution* solution, size_t n);

/**
 *  @brief          Free an allocated \ref KSolution
 *
 *  @param[in]      solution   The solution to free
 */
void kp_free_sol(KSolution solution);

/**
 *  @brief          Initialize a \ref KBestSolutions
 *
 *  @param[out]     solution A pointer to filled with the new kBestSolutions address
 *  @param[in]      problem The KProblem to be set. The problem is copied.
 *  @param[in]      sol_count The number of solutions to be allocated
 */
void kp_init_kbest_sols(KBestSolutions* solution, KProblem problem, size_t sol_count);

/**
 *  @brief          Free an allocated \ref KBestSolutions, including the solution vector.
 *
 *  @param[in]      solution   The solution to free
 */
void kp_free_kbest_sols(KBestSolutions solution);

/**
 *  @brief          Solve the K-Best knapsack problem, finding at most \p K solutions.
 *
 *  @param[out]    dest  A pointer to be filled with the \ref KBestSolutions
 *  @param[in]     problem The problem to solve
 *  @param[in]     K The number of solutions to find
 *
 *  @details        Solves the k-best knapsack problem using the algorithm
 *                  described in: "Yanasse, Soma, Maculan - An Algorithm For
 *                  Determining The K-Best Solutions Of The One-Dimensional
 *                  Knapsack Problem
 *                  (http://dx.doi.org/10.1590/S0101-74382000000100011)"
 */
void kp_solve(KBestSolutions* dest, KProblem problem, size_t K);

#endif /* KBEST_H_ */
