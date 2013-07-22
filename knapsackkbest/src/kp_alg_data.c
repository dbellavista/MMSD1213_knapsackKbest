/*
 * kp_alg.c
 *
 *  Created on: Jul 18, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdlib.h>
#include "../include/kp_alg/kp_alg.h"

void kp_init_inn_sol(InnerSolution* iSol, uint16 n, uint16 j, uint16 t,
uint16 v) {
	*iSol = (InnerSolution) malloc(sizeof(struct innerSolution));
	(*iSol)->dimension = n;
	(*iSol)->column_idx = j;
	(*iSol)->row_idx = t;
	(*iSol)->value = v;
	(*iSol)->recovered = false;
	// Sets the value to 0
	(*iSol)->sol_vector = (uint16*) calloc(n, sizeof(uint16));
}

void kp_free_inn_sol(InnerSolution innerSol) {
	free(innerSol->sol_vector);
	free(innerSol);
}

void kp_copy_inn_sol(InnerSolution* dest, InnerSolution origin) {
	kp_init_inn_sol(dest, origin->dimension, origin->column_idx,
			origin->row_idx, origin->value);
	(*dest)->recovered = origin->recovered;
	uint16 i;
	for (i = 0; i < origin->dimension; i++) {
		(*dest)->sol_vector[i] = origin->sol_vector[i];
	}
}
