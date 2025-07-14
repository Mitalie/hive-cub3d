#include "util.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool	util_err_false(const char *msg, const char *arg)
{
	if (arg)
		printf("Error\n%s: %s\n", msg, arg);
	else
		printf("Error\n%s\n", msg);
	return (false);
}

bool	util_errno_false(const char *msg, const char *arg)
{
	if (arg)
		printf("Error\n%s: %s: %s\n", msg, arg, strerror(errno));
	else
		printf("Error\n%s: %s\n", msg, strerror(errno));
	return (false);
}
