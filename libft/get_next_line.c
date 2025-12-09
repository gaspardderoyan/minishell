/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:26:56 by mgregoir          #+#    #+#             */
/*   Updated: 2025/08/26 14:12:48 by mgregoir         ###   ########.fr       */
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
 * 			   character.
 * @param rest The buffer containing the line to extract.
 * @return     A newly allocated string containing the line, or NULL on error.
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
 * @brief      Reads and returns the next line from a file descriptor.
 * @param fd   The file descriptor to read from.
 * @return     The next line from the file descriptor, including the newline
 *             character if present, or NULL if the end of file is reached or
 *             an error occurs.
 */
char	*get_next_line(int fd)
{
	static char	*rest;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!rest)
		rest = ft_strdup_safe("");
	if (!rest)
		return (NULL);
	rest = read_until_nl(fd, rest);
	if (!rest)
		return (NULL);
	line = extract_line(rest);
	if (!line)
	{
		free(rest);
		rest = NULL;
		return (NULL);
	}
	rest = get_remaining(rest);
	return (line);
}

/* 
#include <stdio.h>
#include <fcntl.h>

int	main(void)
{
	int		fd = open("test.txt", O_RDONLY);
	char	*line;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line)
		{
			printf("line >> %s", line);
			free(line);
		}
	}
	line = get_next_line(42);
	printf("wrong fd >> %s\n", line);
	free(line);

	line = get_next_line(STDIN_FILENO);
	printf("stdin >> %s\n", line);
	free(line);
	
	close(fd);

	return (0);

}
*/
