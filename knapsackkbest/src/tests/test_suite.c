/*
 * test_suite.c
 *
 *  Created on: Aug 5, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdarg.h>
#include <stdio.h>
#include "test_suite.h"

char* results[] = { CRED"failed"CNRM, CGRN"successful"CNRM };

void tests(char* suite_name, ...) {

	va_list arguments;
	va_start(arguments, suite_name);

	printf(CYEL "=================================\n" CNRM);
	printf(CYEL "= TEST SUITE: %s\n" CNRM, suite_name);
	int i = 0;
	bool res, tot_res = true;

	bool (*func)(void);

	while ((func = va_arg(arguments, bool (*)(void))) != NULL) {
		set_up();
		printf(CMAG "\n==== Test no %d of %s...\n" CNRM, (i + 1), suite_name);
		res = (*func)();
		tot_res &= res;
		printf(CMAG "==== Test no %d %s\n" CNRM, (i + 1), results[res]);
		tear_down();
		i++;
	}
	printf("\n");
	if (tot_res) {
		printf(CGRN"******* Success %s: all tests passed!\n\n"CNRM, suite_name);
	} else {
		printf(CRED"******* Failure %s: some tests did not pass!\n\n"CNRM, suite_name);
	}
}
