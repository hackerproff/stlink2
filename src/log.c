/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */

/**
 * @file src/log.c
 */
#include <stlink2/build.h>
#include <stlink2-internal.h>
#include <string.h>

/**
 * Get loglevel string prefix for logline
 */
static const char *stlink2_loglevel_logline_str(const enum stlink2_loglevel level)
{
	switch (level) {
	case STLINK2_LOGLEVEL_ERROR:
		return "[ERROR]";
	case STLINK2_LOGLEVEL_WARN:
		return "[WARN] ";
	case STLINK2_LOGLEVEL_INFO:
		return "[INFO] ";
	case STLINK2_LOGLEVEL_DEBUG:
		return "[DEBUG]";
	case STLINK2_LOGLEVEL_TRACE:
		return "[TRACE]";
	default:
		break;
	}

	return "";
}

/**
 * Convert loglevel string to enum
 */
static enum stlink2_loglevel stlink2_log_level_from_str(const char *level)
{
	if (!level)
		return STLINK2_LOGLEVEL_QUIET;

	if (strcmp(level, "error") == 0)
		return STLINK2_LOGLEVEL_ERROR;
	else if (strcmp(level, "warn") == 0)
		return STLINK2_LOGLEVEL_WARN;
	else if (strcmp(level, "info") == 0)
		return STLINK2_LOGLEVEL_INFO;
	else if (strcmp(level, "debug") == 0)
		return STLINK2_LOGLEVEL_DEBUG;
	else if (strcmp(level, "trace") == 0)
		return STLINK2_LOGLEVEL_TRACE;

	return STLINK2_LOGLEVEL_QUIET;
}

void stlink2_log(enum stlink2_loglevel level, const char *file, const unsigned int line, const char *function, const stlink2_t dev,
		 const char *format, ...)
{
	if (!dev->log.fp || dev->log.level < level)
		return;

	va_list args;

	if (file && line && function)
		fprintf(dev->log.fp, "%s (%s) %s:%u : ", stlink2_loglevel_logline_str(level), function, file, line);

	va_start(args, format);
	vfprintf(dev->log.fp, format, args);
	va_end(args);
}

void stlink2_log_set_default_level(stlink2_context_t ctx, const enum stlink2_loglevel level)
{
	ctx->log.level = level;
}

void stlink2_log_set_default_file(stlink2_context_t ctx, FILE *fp)
{
	ctx->log.fp = fp;
}

void stlink2_log_set_default_level_str(stlink2_context_t ctx, const char *level)
{
	stlink2_log_set_default_level(ctx, stlink2_log_level_from_str(level));
}

void stlink2_log_set_default_filename(stlink2_context_t dev, const char *filename)
{
	if (!filename)
		return;

	if (strcmp(filename, "stdout") == 0)
		stlink2_log_set_default_file(dev, stdout); 
	else if (strcmp(filename, "stderr") == 0)
		stlink2_log_set_default_file(dev, stderr); 
}

void stlink2_log_set_file(stlink2_t dev, FILE *file)
{
	dev->log.fp = file;
}

void stlink2_log_set_filename(stlink2_t dev, const char *filename)
{
	if (!filename)
		return;

	if (strcmp(filename, "stdout") == 0)
		stlink2_log_set_file(dev, stdout); 
	else if (strcmp(filename, "stderr") == 0)
		stlink2_log_set_file(dev, stderr); 
}

void stlink2_log_set_level(stlink2_t dev, enum stlink2_loglevel level)
{
	dev->log.level = level;
}

void stlink2_log_set_level_str(stlink2_t dev, const char *level)
{
	stlink2_log_set_level(dev, stlink2_log_level_from_str(level));
}
