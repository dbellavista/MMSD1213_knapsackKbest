/**
 *
 * @file    kp_algorithms.h
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    10/21/2013 01:26:09 PM
 * @brief   Contains the definition of the algorithm for solving the K-Best Knapsack problem
 *
 * @details This functions are the implementation of "Yanasse, Soma, Maculan -
 *          An Algorithm For Determining The K-Best Solutions Of The One-Dimensional
 *          Knapsack Problem (http://dx.doi.org/10.1590/S0101-74382000000100011)"
 *
 */

#ifndef KP_ALG_H_
#define KP_ALG_H_

#include "kp_model/kp_inner_model.h"
#include "kbest.h"

/**
 *  @brief   Performs the forward enumeration, by filling the matrix.
 *
 *  @param[in]      matrix  The <tt>nxm</tt> matrix to fill. <tt>n</tt> is the maximum
 *                          weight, <tt>m</tt> is the number of variables
 *  @param[in]   problem   The KProblem
 *
 *  @details        First step of the algorithm: ramification of the
 *                  supernodes, by filling the matrix with solution values. An
 *                  element <tt>v</tt> in the matrix cell <tt>ixj</tt> correspond to a
 *                  solution of value <tt>v</tt>, with weight <tt>i</tt> and
 *                  <tt>j</tt> as <tt>weightiest</tt> variable.
 */
void kp_forward_enumeration(int** matrix, KProblem problem);

/**
 *  @brief        Build the initial K-Best solution list, starting from the filled matrix.
 *
 *  @param[out]   ret   A pointer to be filled with an array of solutions.
 *  @param[out]   ret_size   A pointer to be filled with the size of \p ret
 *  @param[in]    matrix   The algorithm matrix
 *  @param[in]    problem   The KProblem
 *  @param[in]    K   The maximum number of solutions to find.
 *
 *  @details      Second step: iterates the matrix from the maximum weight and weightiest
 *                variable finding the best K values (at most) and creating an
 *                InnerSolution for each of them.
 */
void kp_build_initial_best_k_list(InnerSolution** ret, size_t* ret_size, int**
    matrix, KProblem problem, size_t K);

/**
 *  @brief         Recovers the solution vectors and search for alternative
 *                 solutions.
 *
 *  @param[in]   solutions   The array of solutions
 *  @param[in,out]   size   A pointer to the size of \p solutions. If the size changes, \p size is modified.
 *  @param[in]   K    The maximum number of solution to find.
 *  @param[in]   matrix    The algorithm matrix
 *  @param[in]    problem   The KProblem
 *
 *  @details        Last step: recovers the solutions, that is, fills
 *                  InnerSolution.sol_vector. While backtracking, it also
 *                  search for alternative solutions.
 */
void kp_recover_solution(InnerSolution* solutions, size_t* size, size_t K,
    int** matrix, KProblem problem);

/**
 *  @brief         Backtracks the matrix filling the solution vector. Also
 *                search for alternative solutions.
 *
 *  @param[in]   solutions   The array of solutions
 *  @param[in]   dest        The solution to be filled
 *  @param[in]   index       The index of \p dest inside \p solutions.
 *  @param[in,out]   sols_size   A pointer to the size of \p solutions. If the
 *                               size changes, \p size is modified.
 *  @param[in]   K    The maximum number of solution to find.
 *  @param[in]   matrix    The algorithm matrix
 *  @param[in]    problem   The KProblem
 *  @param[in]    alternative   If set to true, it searches also for alternative solutions.
 *
 *  @details     Backtracks the matrix, filling the solution vector of \p dest,
 *               knowing that its position inside \p solutions is \p index.
 *               \ref backtracking assumes that \p dest is equals to
 *               <tt>solutions[index]</tt> (except for the solution vector),
 *               but it's not the same instance.
 */
void backtracking(InnerSolution* solutions, InnerSolution dest, size_t index,
    size_t* sols_size, size_t K, int** matrix, KProblem problem, bool
    alternative);

/**
 *  @brief          Search for alternative solution in the given supernode.
 *
 *  @param[in]   snode     The supernode, that is, the matrix row.
 *  @param[in]   cur_var   The variable (the matrix column) that has just been examined by backtraking.
 *  @param[in]   cum_val   The value of the hypothetical solution so far.
 *  @param[in]   limit_var The maximum variable (maximum column) to search.
 *  @param[in]   sol_index The current solution index in \p solutions
 *  @param[in]   auxl      The current solution that is being filled by backtracking.
 *  @param[in]   solutions The array of solutions
 *  @param[in,out]   sols_size   A pointer to the size of \p solutions. If the
 *                               size changes, \p size is modified.
 *  @param[in]   K         The maximum number of solution to find.
 *  @param[in]   matrix    The algorithm matrix
 *  @param[in]   problem   The KProblem
 *
 *
 *  @details    Search for alternatives solution by addind \p cum_val to every
 *              element of the supernode from \p limit_var down to 0, except \p
 *              cur_var. If the alternative solution value is better than the
 *              last solution, then it backtracks the new solution
 *              <i>WITHOUT</i> recursively searching for alternative solutions.
 *              Then if the new solution is equals to an already recovered
 *              solution, the new solution is discarded.
 */
void search_alternative_solutions(size_t snode, size_t cur_var, uint32_t
    cum_val, size_t limit_var, size_t sol_index, InnerSolution auxl,
    InnerSolution* solutions, size_t* sols_size, size_t K, int** matrix,
    KProblem problem);

#endif /* KP_ALG_H_ */
