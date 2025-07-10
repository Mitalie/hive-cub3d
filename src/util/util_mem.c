#include "util.h"

#include <stdlib.h>

int	util_memcmp(const void *a, const void *b, size_t n)
{
	int					cmp;
	const unsigned char	*a_c;
	const unsigned char	*b_c;

	cmp = 0;
	a_c = a;
	b_c = b;
	while (n-- && cmp == 0)
		cmp = (*a_c++) - (*b_c++);
	return (cmp);
}

void	util_memcpy(void *dst, const void *src, size_t n)
{
	char		*dst_c;
	const char	*src_c;

	dst_c = dst;
	src_c = src;
	while (n--)
		*dst_c++ = *src_c++;
}

void	*util_realloc(void *buf, size_t old_size, size_t new_size)
{
	void	*new_buf;

	if (!buf)
		return (malloc(new_size));
	if (new_size == 0)
	{
		free(buf);
		return (NULL);
	}
	new_buf = malloc(new_size);
	if (!new_buf)
	{
		free(buf);
		return (NULL);
	}
	if (old_size > new_size)
		util_memcpy(new_buf, buf, new_size);
	else
		util_memcpy(new_buf, buf, old_size);
	free(buf);
	return (new_buf);
}
