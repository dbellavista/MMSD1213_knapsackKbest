/**
 *
 * @file    utility.c
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    10/21/2013 12:24:02 PM
 * @brief   General utilities.
 *
 */

#include <stdlib.h>
#include <string.h>
#include "utility.h"

ssize_t find_idx(int* vector, size_t fin, int value)
{
	uint32_t i;
	for (i = 0; i <= fin; i++) {
		if (vector[i] == value) {
			return i;
		}
	}
	return -1;
}

void allocate_matrix(void*** matrix, uint32_t n_row, uint32_t n_cols, uint32_t size)
{
  /// For a more efficient allocation, the matrix is a continuous block
	(*matrix) = (void**) malloc(n_row * sizeof(void*));
	void* vals = (void*) malloc(n_row * n_cols * size);
	uint32_t i;
	for (i = 0; i < n_row; i++) {
		(*(char***)matrix)[i] = &(((char*)vals)[i * n_cols * size]);
	}
}

void free_matrix(void** matrix)
{
	free(matrix[0]);
	free(matrix);
}
