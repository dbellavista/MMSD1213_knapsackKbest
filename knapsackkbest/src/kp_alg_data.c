/*
 * kp_alg.c
 *
 *  Created on: Jul 18, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdlib.h>
#include <string.h>
#include "kp_alg.h"
#include "utility.h"

void kp_init_inn_sol(InnerSolution* iSol, size_t n, size_t j, size_t t,
    uint32_t v)
{
	*iSol = (InnerSolution) malloc(sizeof(struct innerSolution));
	(*iSol)->dimension = n;
	(*iSol)->column_idx = j;
	(*iSol)->row_idx = t;
	(*iSol)->value = v;
	(*iSol)->recovered = false;
	// Sets the value to 0
	(*iSol)->sol_vector = (uint32_t*) calloc(n, sizeof(uint32_t));

	(*iSol)->last_zero = 0;
	(*iSol)->bitmap_sol_vector = (bitmap_t*) calloc(BITMAP_DYNAMIC_BYTE_SIZE(*iSol), sizeof(char));
}

void kp_free_inn_sol(InnerSolution innerSol)
{
	free(innerSol->sol_vector);
	innerSol->sol_vector = NULL;
	free(innerSol->bitmap_sol_vector);
	innerSol->bitmap_sol_vector = NULL;
	free(innerSol);
}

void kp_copy_inn_sol(InnerSolution* dest, InnerSolution origin)
{
	uint32_t i;
	kp_init_inn_sol(dest, origin->dimension, origin->column_idx,
			origin->row_idx, origin->value);
	(*dest)->recovered = origin->recovered;
	for (i = 0; i < origin->dimension; i++) {
		(*dest)->sol_vector[i] = origin->sol_vector[i];
	}
	(*dest)->last_zero = origin->last_zero;
  memcpy((*dest)->bitmap_sol_vector, origin->bitmap_sol_vector, BITMAP_DYNAMIC_BYTE_SIZE(origin));
}
