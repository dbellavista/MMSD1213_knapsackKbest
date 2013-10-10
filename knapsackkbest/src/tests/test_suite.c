/*
 * test_suite.c
 *
 *  Created on: Aug 5, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdarg.h>
#include <stdio.h>
#include "test_suite.h"

char* results[] = { "failed", "successful" };

void tests(char* suite_name, ...) {

	va_list arguments;
	va_start(arguments, suite_name);

	printf("=================================\n");
	printf("= TEST SUITE: %s\n", suite_name);
	int i = 0;
	bool res, tot_res = true;

	bool (*func)(void);

	while ((func = va_arg(arguments, bool (*)(void))) != NULL) {
		set_up();
		printf("\n==== Test no %d of %s...\n", (i + 1), suite_name);
		res = (*func)();
		tot_res &= res;
		printf("==== Test no %d %s\n", (i + 1), results[res]);
		tear_down();
		i++;
	}
	printf("\n");
	if (tot_res) {
		printf("******* Success %s: all tests passed!\n\n", suite_name);
	} else {
		printf("******* Failure %s: some tests did not pass!\n\n", suite_name);
	}
}
