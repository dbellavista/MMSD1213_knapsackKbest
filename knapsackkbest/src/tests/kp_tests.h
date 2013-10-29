/*
 * tests.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef KP_TESTS_H_
#define KP_TESTS_H_

#include <stdio.h>
#include "test_suite.h"
#include "utility/data.h"
#include "kp_model/kp_inner_model.h"
#include "kp_algorithm/kp_algorithms.h"

void do_kp_tests();
bool test_input_file();
bool test_problem_creation();
bool test_matrix_alloc();
bool test_solution_creation();
bool test_kbestsolutions_creation();
bool test_innersol_creation();
bool test_innersol_ordering();
bool test_innersol_join();
bool test_find();
bool test_innersol_copy();
bool test_find_innsol_idx();
bool test_create_kbest_from_inner();
bool test_kp_algorithm();
bool test_kp_solver();

#endif /* KP_TESTS_H_ */
