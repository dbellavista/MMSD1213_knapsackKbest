/**
 *
 * @file    debug.h
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    10/21/2013 03:28:37 PM
 * @brief   Debug print utilities
 *
 */

#ifndef DEBUG_H_
#define DEBUG_H_

/// The debug print level
enum debug_level {
  /// Display notices
	NOTICE = 0x1,
	/// Display warnings
	WARNING = 0x2,
	/// Display errors
	ERROR = 0x4,
	/// Display debug messages
	DEBUG = 0x8
};


/**
 *  @brief          Sets the global debug level
 *
 *  @param[in]   debug   A combination of debug_level
 */
void set_debug_level(unsigned int debug);

/**
 *  @brief          Prints a notice message
 *
 *  @param[in]   format   The format string
 *
 */
void d_notice(char* format, ...);
/**
 *  @brief          Prints a warning message
 *
 *  @param[in]   format   The format string
 *
 */
void d_warning(char* format, ...);
/**
 *  @brief          Prints an error message
 *
 *  @param[in]   format   The format string
 *
 */
void d_error(char* format, ...);
/**
 *  @brief          Prints a warning message
 *
 *  @param[in]   format   The format string
 *
 */
void d_debug(char* format, ...);

/**
 *  @brief          Increments a global indentation level
 *
 */
void d_inc_indent();
/**
 *  @brief          Decrements the global indentation level
 *
 */
void d_dec_indent();

#endif /* DEBUG_H_ */
