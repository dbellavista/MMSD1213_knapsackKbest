/*
 * test_suite.h
 *
 *  Created on: Aug 5, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef TEST_SUITE_H_
#define TEST_SUITE_H_

#include "../include/data.h"

void set_up();
void tear_down();
void tests(char* suite_name, ...);

#endif /* TEST_SUITE_H_ */
