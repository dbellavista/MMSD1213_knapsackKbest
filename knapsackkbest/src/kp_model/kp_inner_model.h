/**
 *
 * @file    kp_inner_model.h
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    10/21/2013 12:32:05 PM
 * @brief   Contains the model structure and utility function definition for
 *          the model used by the internal algorithms.
 *
 */

#ifndef KP_INNER_MODEL_H_
#define KP_INNER_MODEL_H_

#include <limits.h>

#include "kbest.h"

#if defined(__x86_64__) || defined(_M_X64)
/// Bitmap type, used for solution comparation
typedef unsigned long long bitmap_t;
#else
/// Bitmap type, used for solution comparation
typedef unsigned long bitmap_t;
#endif

/// Returns the size in bitmap_t of InnerSolution.bitmap_sol_vector
#define BITMAP_DYNAMIC_SIZE(sol) CEIL((sol)->dimension / 8 / sizeof(bitmap_t), size_t)

/// An internal, potentially uncompleted solution. Used during the forward and
/// backtracking phases.
struct innerSolution {
  /// The solution vector containing the number of elements for each variables
	uint32_t* sol_vector;
	/// The number of variables
	size_t dimension;
	/// The row position (weight) in the algorithm matrix.
	size_t row_idx;
	/// The column position (variable) in the algorithm matrix.
	size_t column_idx;
	/// The value of the solution
	uint32_t value;
	/// Set to true if this solution is recovered, that is \p sol_vector is full.
	bool recovered;

  /// Bitmap of the sol_vector. A bit is set iff the corresponding variable as
  /// a value in \p sol_vector greather than 0.
  bitmap_t *bitmap_sol_vector;
  /// The index of the zero-variable after the last non-zero variable in \p sol_vector.
  size_t last_zero;
};
/// A pointer to \p struct innerSolution
typedef struct innerSolution* InnerSolution;

/**
 *  @brief          Initialize a \p InnerSolution
 *
 *  @param[out]   innerSol   The pointer to be filled with the new inner solution.
 *  @param[in]   n     The number of variables
 *  @param[in]   j   The column idx in the algorithm matrix (variable).
 *  @param[in]   t   The row idx in the algorithm matrix (weight).
 *  @param[in]   v   The solution value.
 */
void kp_init_inn_sol(InnerSolution* innerSol, size_t n, size_t j, size_t t, uint32_t v);

/**
 *  @brief          Free a \p InnerSolution
 *
 *  @param[in]   innerSol   The solution to free
 */
void kp_free_inn_sol(InnerSolution innerSol);

/**
 *  @brief          Create a copy of an \p InnerSolution
 *
 *  @param[out]   dest   The pointer to be filled with the new innner solution.
 *  @param[in]      origin  The solution to copy
 */
void kp_copy_inn_sol(InnerSolution* dest, InnerSolution origin);

/**
 *  @brief          Generate a KBestSolutions starting from an array of InnerSolution.
 *
 *  @param[out]      dest A pointer to be filled with the new KBestSolutions
 *  @param[in]      solutions The array of InnerSolution
 *  @param[in]   solutions_size   The size of \p solutions
 *  @param[in]   problem   The original KProblem
 *  @param[in]   free_inner   If set to true, the array of InnerSolution and its components will be freed.
 *
 */
void create_kbest_solutions_from_inner(KBestSolutions* dest, InnerSolution*
    solutions, size_t solutions_size, KProblem problem, bool free_inner);


/**
 *  @brief          Returns true if the given InnerSolution are equals.
 *
 *  @param[in]      sol1    The first InnerSolution
 *  @param[in]      sol2    The second InnerSolution
 *
 *  @return         Returns true if the given InnerSolution are equals. That is
 *                  if they are both recovered and they have the same solution vector.
 *
 *  @details        To optimize the check, the functions uses last_zero and
 *                  bitmap to quickly discards solutions that have the same
 *                  value but different solutions vector. For instance:
 *                  @code
 *                  sv1 = [5 4 2 0 8 9 0 12 0 0 0 0]
 *                  sv2 = [5 4 2 0 8 9 0 11 1 0 0 0]
 *                  @endcode
 *                  have a different \ref InnerSolution.last_zero value. For
 *                  <tt>sv1</tt> it's 8 and for <tt>sv2</tt> it's 9. While:
 *                  @code
 *                  sv3 = [5 4 7 0 9 0 0 8 4 0 0 0]
 *                  sv4 = [5 4 2 0 8 9 0 11 1 0 0 0]
 *                  @endcode
 *                  Have the same \ref InnerSolution.last_zero value (9), but
 *                  the bitmap is different (in bits):
 *                  @code
 *                  sv3.bitmap = 1110100110000000
 *                  sv4.bitmap = 1110110110000000
 *                  @endcode
 *
 */
bool inner_solutions_equal(InnerSolution sol1, InnerSolution sol2);


/**
 *  @brief          Joins two inner solution array inside a new solution of size at
 *                  most \p K.
 *
 *  @param[in]   dest   The array that will contain the joined result. It must
 *                       have at least size of K.
 *  @param[in]   sol1   The first solution to join
 *  @param[in]   sol2   The second solution to join
 *  @param[in]   count1   The size of \p sol1
 *  @param[in]   count2   The size of \p sol2
 *  @param[in]   K   The maximum size of \p dest.
 *  @param[in]   free_garbage   If true, the inner solutions excluded from \p dest are freed.
 *
 *  @details     Join two array of inner solutions sorted by decreasing value,
 *               inside \p dest. \p dest must be preallocated, with size at least K.
 */
void join_inner_solutions(InnerSolution* dest, InnerSolution* sol1,
    InnerSolution* sol2, size_t count1, size_t count2, size_t K, bool
    free_garbage);


/**
 *  @brief          Sorts the given array by non increasing value of \ref InnerSolution.value
 *
 *  @param[in]   sol   The array of InnerSolution to sort
 *  @param[in]   count   The size of \p sol.
 */
void sort_by_values_non_inc(InnerSolution* sol, uint32_t count);


/**
 *  @brief          Find the index for the insertion of a solution of value \p
 *                  value in the sorted list sol_list]lower_limit:size[
 *
 *  @param[in]   sol_list   The sorted array of InnerSolution to iterate
 *  @param[in]   sols_size   The size of sol_list
 *  @param[in]   size_max   The maximum size allowed
 *  @param[in]   lower_limit_idx   The exclusive start index (-1 for searching from the beginning).
 *  @param[in]   value  The value to be inserted
 *
 *  @return      The index where the element should be inserted, or -1 if the insertion is not possible.
 */
ssize_t find_idx_insertion(InnerSolution* sol_list, size_t sols_size, size_t
    size_max, ssize_t lower_limit_idx, uint32_t value);


/**
 *  @brief          Finds the index to insert the value \p value and move the
 *                  array elements to free the insertion index.
 *
 *  @param[in]   sol_list   The solution list
 *  @param[in,out]  sols_size   A pointer to the solution size. If the size is
 *                              modified, sols_size is modified.
 *  @param[in,out]  removed    Optional. A pointer to be filled with the
 *                              removed element in case the index is found and
 *                              \p sols_size is equal to \p K.
 *  @param[in]  lower_limit_idx    The exclusive start index
 *  @param[in]   value  The value to be inserted
 *  @param[in]  K    The array maximum size.
 *
 *  @return      The index where the element has to be inserted, or -1 if the insertion was not possible.
 *
 *  @details     This function calls find_idx_insertion to find the idx. If the
 *               index exists, it calls prepare_insertion to move the elements.
 *
 */
ssize_t find_idx_and_prepare_insertion(InnerSolution* sol_list, size_t*
    sols_size, InnerSolution* removed, ssize_t lower_limit_idx, uint32_t value,
    size_t K);

/**
 *  @brief          Move the array elements to free the insertion index.
 *
 *  @param[in]   sol_list   The solution list
 *  @param[in,out]  sols_size   A pointer to the solution size. If the size is
 *                              modified, sols_size is modified.
 *  @param[in]   insert_idx   The index of insertion
 *  @param[in,out]  removed    Optional. A pointer to be filled with the
 *                              removed element in case the index is found and
 *                              \p sols_size is equal to \p K.
 *  @param[in]  K    The array maximum size.
 *
 *  @details     It moves the elements from index + 1 to sol_size
 *               toward right. If sols_size is equal to K, then there is no
 *               space for the last element: if \p removed is not NULL, then
 *               the function set its value to the last element, otherwise the
 *               last element is freed.
 */
void prepare_insertion(InnerSolution* sol_list, size_t* sols_size, size_t insert_idx,
    InnerSolution* removed, size_t K);

/**
 *  @brief          Sum the \ref InnerSolution.sol_vector of \p s1 and \p s2, putting the
 *                  results in \p dest.
 *
 *  @param[in]   dest   The destination solution
 *  @param[in]   s1   The first solution
 *  @param[in]   s2   The second solution
 */
void sum_solution_vectors(InnerSolution dest, InnerSolution s1, InnerSolution
    s2);


/**
 *  @brief          Set the element of InnerSolution.sol_vector to a certain
 *  value, changing if necessary InnserSolution.last_zero and
 *  InnerSolution.bitmap_sol_vector
 *
 *  @param[in]    isol  The inner solution
 *  @param[in]    var   The variable, that is the index in the solution vector
 *  @param[in]    value   The value to set.
 *
 */
void set_inner_sol_element(InnerSolution isol, size_t var, uint32_t value);

/**
 *  @brief          Debug/Utility function for printing to stdout an InnerSolution
 *
 *  @param[in]   sol   The solution to print
 */
void print_inner_solution(InnerSolution sol);

#endif /* KP_INNER_MODEL_H_ */
