/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:26:56 by mgregoir          #+#    #+#             */
/*   Updated: 2025/08/26 14:16:11 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Extracts the remaining text after the first newline.
 * @param rest The buffer containing the current line and remaining text.
 * @return     A new string containing the text after the newline, or NULL if
 *             there is no remaining text.
 */
static char	*get_remaining(char *rest)
{
	char	*new_rest;
	size_t	i;

	i = 0;
	while (rest[i] && rest[i] != '\n')
		i++;
	if (rest[i] == '\0' || rest[i + 1] == '\0')
		return (free(rest), NULL);
	new_rest = ft_strdup_safe(rest + i + 1);
	if (!new_rest)
		return (free(rest), NULL);
	free(rest);
	return (new_rest);
}

/**
 * @brief      Extracts one line from the buffer, including the newline 
 *             character.
 * @param rest The buffer containing the line to extract.
 * @return     A newly allocated string containing the line, or NULL 
 *             on error.
 */
static char	*extract_line(char *rest)
{
	char	*line;
	size_t	i;

	i = 0;
	if (!rest || !rest[i])
		return (NULL);
	while (rest[i] && rest[i] != '\n')
		i++;
	if (rest[i] == '\n')
		i++;
	line = malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (rest[i] && rest[i] != '\n')
	{
		line[i] = rest[i];
		i++;
	}
	if (rest[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

/**
 * @brief      Reads from a file descriptor until a newline is found or EOF.
 * @param fd   The file descriptor to read from.
 * @param rest The existing buffer to append to.
 * @return     The updated buffer containing the read content, or NULL on error.
 */
static char	*read_until_nl(int fd, char *rest)
{
	char	*buffer;
	int		bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr_safe(rest, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(buffer), free(rest), NULL);
		buffer[bytes_read] = '\0';
		rest = ft_strjoin_gnl(rest, buffer);
		if (!rest)
			return (free(buffer), NULL);
	}
	free(buffer);
	return (rest);
}

/**
 * @brief      Reads and returns the next line from a file descriptor 
 *             (bonus: multiple FDs).
 * @param fd   The file descriptor to read from.
 * @return     The next line from the file descriptor, including the newline
 *             character if present, or NULL if the end of file is reached or
 *             an error occurs. Supports reading from multiple file descriptors.
 */
char	*get_next_line(int fd)
{
	static char	*rest[MAX_FD];
	char		*line;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	if (!rest[fd])
		rest[fd] = ft_strdup_safe("");
	if (!rest[fd])
		return (NULL);
	rest[fd] = read_until_nl(fd, rest[fd]);
	if (!rest[fd])
		return (NULL);
	line = extract_line(rest[fd]);
	if (!line)
	{
		free(rest[fd]);
		rest[fd] = NULL;
		return (NULL);
	}
	rest[fd] = get_remaining(rest[fd]);
	return (line);
}
/*
#include <stdio.h>
#include <fcntl.h>

int	main(void)
{
	int		fd1 = open("test.txt", O_RDONLY);
	int		fd2 = open("book.txt", O_RDONLY);
	char	*line1;
	char	*line2;

	while (1)
	{
		line1 = get_next_line(fd1);
		line2 = get_next_line(fd2);
		if (!line1 && !line2)
			break ;
		if (line1)
		{
			printf("line1 >> %s", line1);
			free(line1);
		}
		if (line2)
		{
			printf("line2 >> %s", line2);
			free(line2);
		}
	}
	close(fd1);
	close(fd2);
	return (0);

}
*/
