/*
 * utility.c
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include "../include/kp_alg/utility.h"

void sort_by_weights(KProblem problem) {

	byte done = 0;
	uint16 i, tmp;
	while (!done) {
		done = 1;
		for (i = 0; i < problem->n - 1; i++) {
			if (problem->weights[i] > problem->weights[i + 1]) {
				done = 0;
				tmp = problem->weights[i];
				problem->weights[i] = problem->weights[i + 1];
				problem->weights[i + 1] = tmp;
				tmp = problem->values[i];
				problem->values[i] = problem->values[i + 1];
				problem->values[i + 1] = tmp;
			}
		}
	}

}
