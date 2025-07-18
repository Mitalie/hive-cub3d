#include "map.h"
#include "map_internal.h"

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "MLX42/MLX42.h"

#include "util.h"

void	map_unload(t_map *map)
{
	mlx_delete_texture(map->wall_north);
	mlx_delete_texture(map->wall_south);
	mlx_delete_texture(map->wall_east);
	mlx_delete_texture(map->wall_west);
	free(map->grid);
}

#define READ_SIZE 4096

/*
	Read more data from the file descriptor, reallocing the buffer if necessary.
	Returns true for EOF or error, false for more data available. In case of
	of error, buffer is freed and buffer pointer is set to NULL.
*/
static bool	map_read_helper(
	int fd, char **buf, size_t *buf_len, size_t *file_len)
{
	ssize_t	read_bytes;
	size_t	new_len;

	if (*buf_len < *file_len + READ_SIZE + 1)
	{
		new_len = *buf_len * 2;
		if (new_len == 0)
			new_len = READ_SIZE * 2;
		*buf = util_realloc(*buf, *buf_len, new_len);
		*buf_len = new_len;
	}
	if (!*buf)
		return (true);
	read_bytes = read(fd, *buf + *file_len, READ_SIZE);
	if (read_bytes < 0)
	{
		free(*buf);
		*buf = NULL;
		return (true);
	}
	*file_len += read_bytes;
	return (read_bytes == 0);
}

static char	*map_load_file(const char *path)
{
	int		fd;
	char	*buf;
	size_t	buf_len;
	size_t	file_len;
	int		saved_errno;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	buf = NULL;
	buf_len = 0;
	file_len = 0;
	while (true)
	{
		if (map_read_helper(fd, &buf, &buf_len, &file_len))
			break ;
	}
	if (buf)
		buf[file_len] = '\0';
	saved_errno = errno;
	close(fd);
	errno = saved_errno;
	return (buf);
}

bool	map_load(t_map *map, const char *path)
{
	char	*file_data;
	bool	success;
	int		i;

	i = 0;
	while (path[i])
		i++;
	if (util_memcmp(".cub", &path[i - 4], 4))
		return (util_err_false("Map filename should end \".cub\"", path));
	file_data = map_load_file(path);
	if (!file_data)
		return (util_errno_false("Loading map file failed", path));
	map->wall_north = NULL;
	map->wall_south = NULL;
	map->wall_east = NULL;
	map->wall_west = NULL;
	map->color_ceil = 0;
	map->color_floor = 0;
	map->player_start = '\0';
	success = map_parse(map, file_data);
	free(file_data);
	return (success);
}
