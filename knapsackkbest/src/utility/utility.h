/**
 *
 * @file    utility.h
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    10/21/2013 12:24:02 PM
 * @brief   General utilities.
 *
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include "data.h"

/// Returns the max value between a and b
#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

/// Returns the absolute value of a
#define ABS(a) \
  ((a < 0) ? -a : a)

/// Returns the min value between a and b
#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

/// Returns the ceiling of a, casted to TYPE
#define CEIL(x, TYPE) \
	( (x) >=0 ? (TYPE) ((x) + 1.0) : (TYPE) ((x) - 1.0))

/// Returns the floor of a, casted to TYPE
#define FLOOR(x, TYPE) \
	( (TYPE) (x) )

/// Returns the round of a, casted to TYPE
#define ROUND(x, TYPE) \
	( (x) >=0 ? (TYPE) ((x) + 0.5) : (TYPE) ((x) - 0.5))

/// Returns the round of a with an upper bound of LIMIT
#define ROUND_LIMIT(x, LIMIT) \
  ((x) < 0 ? 0 : (x) >= (LIMIT) ? (LIMIT) - 1 : (unsigned long) ((x) + 0.5))

/// Compare two doubles
#define DOUBLE_EQUALS(x, y) \
	((x >= y - DBL_EPSILON) && (x <= y + DBL_EPSILON))

/// Compare x with 0.0
#define DZERO(x) DOUBLE_EQUALS(x, 0.0)

/// Returns X*X
#define SQR(X) (X)*(X)


/**
 *  @brief          Allocate a \p nrow x \p ncols matrix of elements of size \p size.
 *
 *  @param[in]      matrix  A pointer to be filled with the created matrix
 *  @param[in]     nrow  The number of row
 *  @param[in]  ncol  The number of col
 *  @param[in]  size  The size of the element of the matrix
 *
 */
void allocate_matrix(void*** matrix, uint32_t nrow, uint32_t ncol, uint32_t size);

/**
 *  @brief          Free a previously allocated matrix with allocate_matrix
 *
 *  @param[in]      matrix The matrix to free
 */
void free_matrix(void** matrix);

/**
 *  @brief          Search \p value in vector[0:fin]
 *
 *  @param[in]    vector  The vector to be iterated
 *  @param[in]       fin     The upper bound index (inclusive)
 *  @param[in]     value The value to be searched
 *
 *  @return         The index of the value in vector[0:fin] or -1 if the value is not found.
 *
 */
ssize_t find_idx(int* vector, size_t fin, int value);

#endif /* UTILITY_H_ */
