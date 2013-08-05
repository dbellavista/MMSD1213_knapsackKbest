/*
 * input.h
 *
 *  Created on: Aug 5, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef INPUT_H_
#define INPUT_H_

#include "kbest.h"

bool read_problem(KProblem* dest, char* file);
void print_kbest_solution(KBestSolutions solution);


#endif /* INPUT_H_ */
