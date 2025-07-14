#ifndef UTIL_H
# define UTIL_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

bool	util_err_false(const char *msg, const char *arg);
bool	util_errno_false(const char *msg, const char *arg);

int		util_memcmp(const void *a, const void *b, size_t n);
void	util_memcpy(void *dst, const void *src, size_t n);
void	*util_realloc(void *buf, size_t old_size, size_t new_size);

bool	util_parse_uint8(char **str, uint8_t *value);

#endif
