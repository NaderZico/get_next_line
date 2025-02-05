/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 14:54:34 by nakhalil          #+#    #+#             */
/*   Updated: 2025/02/05 20:38:43 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*extract_line(char *remainder)
{
	size_t	i;
	char	*line;

	i = 0;
	while (remainder[i] && remainder[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + (remainder[i] == '\n') + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (remainder[i] && remainder[i] != '\n')
	{
		line[i] = remainder[i];
		i++;
	}
	if (remainder[i] == '\n')
	{
		line[i] = '\n';
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*save_remainder(char *buffer)
{
	size_t	i;
	size_t	j;
	char	*remainder;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
		return (NULL);
	remainder = malloc(sizeof(char) * (ft_strlen(buffer) - i));
	if (!remainder)
		return (NULL);
	i++;
	j = 0;
	while (buffer[i])
	{
		remainder[j] = buffer[i];
		i++;
		j++;
	}
	remainder[j] = '\0';
	return (remainder);
}

static int	add_to_remainder(char **remainder, char *buffer,
		ssize_t bytes_read)
{
	char	*temp;

	buffer[bytes_read] = '\0';
	temp = ft_strjoin(*remainder, buffer);
	if (!temp)
	{
		free_pointer(remainder);
		return (0);
	}
	free(*remainder);
	*remainder = temp;
	return (1);
}

static int	read_and_store(int fd, char **remainder)
{
	char	*buffer;
	ssize_t	bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (free_pointer(remainder));
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		if (!add_to_remainder(remainder, buffer, bytes_read))
		{
			free(buffer);
			return (0);
		}
		if (ft_strchr(*remainder, '\n'))
			break ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	return (bytes_read >= 0);
}

char	*get_next_line(int fd)
{
	static char	*remainder;
	char		*line;
	char		*temp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!remainder)
	{
		remainder = malloc(sizeof(char));
		if (!remainder)
			return (NULL);
		remainder[0] = '\0';
	}
	if (!read_and_store(fd, &remainder) || remainder[0] == '\0')
		return (free_pointer(&remainder), NULL);
	line = extract_line(remainder);
	if (!line)
		return (free_pointer(&remainder), NULL);
	temp = save_remainder(remainder);
	free_pointer(&remainder);
	remainder = temp;
	return (line);
}
