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
#include "../include/kp_alg/kp_alg.h"

void set_up();
void tear_down();
void do_tests();
bool test_problem_creation();
bool test_matrix_alloc();
bool test_solution_creation();
bool test_kbestsolutions_creation();
bool test_innersol_creation();
bool test_innersol_ordering();
bool test_innersol_join();
bool test_find();
bool test_innersol_copy();

#endif /* KP_TESTS_H_ */
