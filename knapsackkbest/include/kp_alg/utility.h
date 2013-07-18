/*
 * utility.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include "../kbest.h"

void sort_by_weights(KProblem problem);
void allocate_matrix(void*** matrix, uint32 width, uint32 height, uint32 size);
void free_matrix(void** matrix);

#endif /* UTILITY_H_ */
