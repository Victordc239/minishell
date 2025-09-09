/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:55:58 by sofernan          #+#    #+#             */
/*   Updated: 2025/07/07 14:13:48 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*free_memory(char **s)
{
	free(*s);
	*s = NULL;
	return (NULL);
}

char	*get_first_line(char *file_cont)
{
	char	*line;
	char	*ptr_line_break;
	int		line_len;

	ptr_line_break = ft_strchr(file_cont, '\n');
	line_len = (ptr_line_break - file_cont) + 1;
	line = ft_substr(file_cont, 0, line_len);
	if (!line)
		return (NULL);
	return (line);
}

char	*remove_first_line(char *file_cont)
{
	char	*rest_cont;
	char	*ptr_line_break;
	int		line_len;

	ptr_line_break = ft_strchr(file_cont, '\n');
	if (!ptr_line_break)
	{
		rest_cont = NULL;
		free_memory(&file_cont);
		return (NULL);
	}
	else
		line_len = (ptr_line_break - file_cont) + 1;
	if (!file_cont[line_len])
	{
		free_memory(&file_cont);
		return (NULL);
	}
	rest_cont = ft_substr(file_cont, line_len, ft_strlen(file_cont) - line_len);
	free_memory(&file_cont);
	if (!rest_cont)
		return (NULL);
	return (rest_cont);
}

char	*add_file_cont(int fd, char *file_cont)
{
	int		bytes_read;
	char	*buffer;

	bytes_read = 1;
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (free_memory(&file_cont));
	buffer[0] = '\0';
	while (bytes_read > 0 && !ft_strchr(buffer, '\n'))
	{
		bytes_read = read (fd, buffer, BUFFER_SIZE);
		if (bytes_read > 0)
		{
			buffer[bytes_read] = '\0';
			file_cont = ft_strjoin_gnl(file_cont, buffer);
		}
	}
	free(buffer);
	buffer = NULL;
	if (bytes_read == -1)
		return (free_memory(&file_cont));
	return (file_cont);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*file_content;

	if (fd < 0)
		return (NULL);
	if ((file_content && !ft_strchr(file_content, '\n')) || !file_content)
		file_content = add_file_cont (fd, file_content);
	if (!file_content)
		return (NULL);
	line = get_first_line(file_content);
	if (!line)
	{
		free_memory(&file_content);
		return (NULL);
	}
	file_content = remove_first_line(file_content);
	return (line);
}
