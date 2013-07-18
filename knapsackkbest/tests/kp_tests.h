/*
 * tests.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef KP_TESTS_H_
#define KP_TESTS_H_

#include <stdio.h>
#include "../include/kbest.h"


void set_up();
void tear_down();
void do_tests();
bool test_problem_creation();
bool test_solution_creation();


#endif /* KP_TESTS_H_ */
