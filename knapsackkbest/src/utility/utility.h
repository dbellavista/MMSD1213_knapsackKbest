/*
 * utility.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include "data.h"

#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define ABS(a) \
  ((a < 0) ? -a : a)

#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define CEIL(x, TYPE) \
	( (x) >=0 ? (TYPE) ((x) + 1.0) : (TYPE) ((x) - 1.0))

#define FLOOR(x, TYPE) \
	( (TYPE) (x) )

#define ROUND(x, TYPE) \
	( (x) >=0 ? (TYPE) ((x) + 0.5) : (TYPE) ((x) - 0.5))

#define ROUND_LIMIT(x, LIMIT) \
  ((x) < 0 ? 0 : (x) >= (LIMIT) ? (LIMIT) - 1 : (unsigned long) ((x) + 0.5))

#define DOUBLE_EQUALS(x, y) \
	((x >= y - DBL_EPSILON) && (x <= y + DBL_EPSILON))

#define DZERO(x) DOUBLE_EQUALS(x, 0.0)

#define SQR(X) (X)*(X)

void allocate_matrix(void*** matrix, uint32_t nrow, uint32_t ncols, uint32_t size);

void free_matrix(void** matrix);

ssize_t find_idx(int* vector, size_t fin, int value);

#endif /* UTILITY_H_ */
