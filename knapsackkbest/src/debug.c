/*
 * debug.c
 *
 *  Created on: Jul 28, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#include <stdarg.h>
#include <stdio.h>
#include "../include/debug.h"

static unsigned int debug_level = NOTICE ^ WARNING ^ ERROR ^ DEBUG;
static int indent = 0;

void printIndent(FILE* stream) {
	int i;
	for(i = 0; i < indent; i++) {
		fprintf(stream, "\t");
	}
}

void set_debug_level(unsigned int debug) {
	debug_level = debug;
}

void d_notice(char* format, ...) {
	if (debug_level & NOTICE) {
		va_list argptr;
		va_start(argptr, format);
		printIndent(stdout);
		fprintf(stdout, "- NOTICE: ");
		vfprintf(stdout, format, argptr);
		va_end(argptr);
	}
}

void d_warning(char* format, ...) {
	if (debug_level & WARNING) {
		va_list argptr;
		va_start(argptr, format);
		printIndent(stdout);
		fprintf(stdout, "+ WARNING: ");
		vfprintf(stdout, format, argptr);
		va_end(argptr);
	}
}

void d_error(char* format, ...) {
	if (debug_level & ERROR) {
		va_list argptr;
		va_start(argptr, format);
		printIndent(stdout);
		fprintf(stdout, "* ERROR: ");
		vfprintf(stdout, format, argptr);
		va_end(argptr);
	}
}

void d_debug(char* format, ...) {
	if (debug_level & DEBUG) {
		va_list argptr;
		va_start(argptr, format);
		printIndent(stdout);
		fprintf(stdout, "^ DEBUG: ");
		vfprintf(stdout, format, argptr);
		va_end(argptr);
	}
}

void d_inc_indent() {
	indent++;
}
void d_dec_indent() {
	if (indent) {
		indent--;
	}
}
