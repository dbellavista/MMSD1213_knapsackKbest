/*
 * debug.h
 *
 *  Created on: Jul 28, 2013
 *      Author: Daniele Bellavista <daniele.bellavista@studio.unibo.it>
 */

#ifndef DEBUG_H_
#define DEBUG_H_

enum debug_level {
	NOTICE = 0x1,
	WARNING = 0x2,
	ERROR = 0x4,
	DEBUG = 0x8
};

void set_debug_level(unsigned int debug);
void d_notice(char* format, ...);
void d_warning(char* format, ...);
void d_error(char* format, ...);
void d_debug(char* format, ...);

void d_inc_indent();
void d_dec_indent();

#endif /* DEBUG_H_ */
