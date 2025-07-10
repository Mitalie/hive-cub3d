#ifndef UTIL_H
# define UTIL_H

# include <stddef.h>

void	util_memcpy(void *dst, const void *src, size_t n);
void	*util_realloc(void *buf, size_t old_size, size_t new_size);

#endif
