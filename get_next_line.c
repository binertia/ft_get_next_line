#include "get_next_line.h"

static char *update_stash(char *p_stash)
{
	char *cache;
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (p_stash[i] && p_stash[i] != '\n')
		i++;
	if (!p_stash[i])
	{
		free(p_stash);
		return (NULL);
	}
	cache = ft_calloc(ft_strlen(p_stash + i), 1);
	if (!cache)
		return (NULL);
	i += 1;
	while (p_stash[i])
		cache[j++] = p_stash[i++];
	free(p_stash);
	return (cache);
}

static char *add_stash(char *p_stash, char *new_buffer)
{
	char *new_stash;

	new_stash = ft_strjoin(p_stash, new_buffer);
	free(p_stash);
	return (new_stash);
}

static char *get_stash(char *p_stash, int fd)
{
	char *buffer;
	int byte_read;
	int i;

	if (!p_stash)
		p_stash = ft_calloc(1,1);
	i = 0;
	byte_read = 1;
	buffer = ft_calloc(BUFFER_SIZE + 2, 1);
	if (!buffer)
		return (NULL);
	while (byte_read > 0)
	{
		byte_read = read(fd, buffer, BUFFER_SIZE);
		if (byte_read == -1)
		{
			free(buffer);
			return (NULL);
		}
		buffer[byte_read] = 0;
		p_stash = add_stash(p_stash, buffer);
		while (i < byte_read && buffer[i] != '\n')
			i++;
		if (i != byte_read)
			break;
	}
	free(buffer);
	return (p_stash);
}

static char *get_line(char *p_stash)
{
	char	*result;
	int i;

	i = 0;
	if (!p_stash[i])
		return (NULL);
	while (p_stash[i] && p_stash[i] != '\n')
		i++;
	result = ft_calloc(i + 2, 1);
	if (!result)
		return (NULL);
	i = 0;
	while (p_stash[i] && p_stash[i] != '\n')
	{
		result[i] = p_stash[i];
		i++;
	}
	if (p_stash[i] && p_stash[i] == '\n')
		result[i] = p_stash[i];
	return (result);
}

char *get_next_line(int fd)
{
	static char *stash;
	char 	*current;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	stash = get_stash(stash, fd);
	if (!stash)
		return (NULL);
	current = get_line(stash);
	stash = update_stash(stash);
	return (current);
}
