/*
 * kp_alg.c
 *
 *  Created on: Jul 18, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdlib.h>
#include "kp_alg.h"

void kp_init_inn_sol(InnerSolution* iSol, uint32_t n, uint32_t j, uint32_t t,
uint32_t v) {
	*iSol = (InnerSolution) malloc(sizeof(struct innerSolution));
	(*iSol)->dimension = n;
	(*iSol)->column_idx = j;
	(*iSol)->row_idx = t;
	(*iSol)->value = v;
	(*iSol)->recovered = false;
	// Sets the value to 0
	(*iSol)->sol_vector = (uint32_t*) calloc(n, sizeof(uint32_t));
}

void kp_free_inn_sol(InnerSolution innerSol) {
	free(innerSol->sol_vector);
	innerSol->sol_vector = NULL;
	free(innerSol);
}

void kp_copy_inn_sol(InnerSolution* dest, InnerSolution origin) {
	kp_init_inn_sol(dest, origin->dimension, origin->column_idx,
			origin->row_idx, origin->value);
	(*dest)->recovered = origin->recovered;
	uint32_t i;
	for (i = 0; i < origin->dimension; i++) {
		(*dest)->sol_vector[i] = origin->sol_vector[i];
	}
}
