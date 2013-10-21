/*
 * test_suite.h
 *
 *  Created on: Aug 5, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef TEST_SUITE_H_
#define TEST_SUITE_H_

#include "utility/data.h"

#define CNRM  "\x1B[0m"
#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CYEL  "\x1B[33m"
#define CBLU  "\x1B[34m"
#define CMAG  "\x1B[35m"
#define CCYN  "\x1B[36m"
#define CWHT  "\x1B[37m"

#define PRINT_INTRO(S) printf(CBLU"%s%s\n"CNRM, S, __FUNCTION__)

void set_up();
void tear_down();
void tests(char* suite_name, ...);

#endif /* TEST_SUITE_H_ */
