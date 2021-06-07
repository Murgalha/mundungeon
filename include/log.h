#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>

#define FOREACH_LVL(LOG_LVL)					\
	LOG_LVL(INFO)								\
	LOG_LVL(ERROR)								\
	LOG_LVL(DEBUG)								\

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

typedef enum {
	FOREACH_LVL(GENERATE_ENUM)
} LOG_LVL;

static const char *loglvl_string[] = {
	FOREACH_LVL(GENERATE_STRING)
};

// TODO: Make macro that receives filename to log onto
#ifdef LOG_COLORED_OUTPUT
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"

static const char *loglvl_color[] = {
	ANSI_COLOR_YELLOW, ANSI_COLOR_RED, ANSI_COLOR_MAGENTA
};

#define log_print(lvl, fmt, ...)									\
	printf("%s%s: " ANSI_COLOR_RESET "%s: " fmt,						\
		   loglvl_color[lvl], loglvl_string[lvl], __FILE__, __VA_ARGS__);
#else

#define log_print(lvl, fmt, ...)									\
	printf("%s: %s: " fmt, loglvl_string[lvl], __FILE__, __VA_ARGS__);
#endif

#endif
