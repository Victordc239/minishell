/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   7.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:40:52 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/18 17:22:30 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	heredoc_signal(int sing)
{
	(void)sing;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	g_status = 130;
	close(0);
}

int	is_limiter(char *line, char *limiter)
{
	if (!limiter)
		return (0);
	if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
		&& line[ft_strlen(limiter)] == '\n')
		return (1);
	return (0);
}

int	process_heredoc(int fd, char *limiter)
{
	char	*line;

	while (1)
	{
		if (g_status == 130)
			return (130);
		write(1, "> ", 2);
		line = get_next_line(0);
		if (g_status == 130)
		{
			free(line);
			return (130);
		}
		if (!line || ft_strcmp(line, limiter) == 0 || is_limiter(line, limiter))
		{
			if (!line)
				ft_putstr("heredoc delimited by EOF\n", 2);
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (0);
}

int	here_doc(char *limiter, char const *filename)
{
	int	fd;
	int	save_in;
	int	result;

	save_in = dup(STDIN_FILENO);
	signal(SIGINT, heredoc_signal);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		close(save_in);
		return (-1);
	}
	if (!limiter || !*limiter)
		exit_with_error(SYNTAX_ERROR, 1, 2);
	result = process_heredoc(fd, limiter);
	close(fd);
	if (dup2(save_in, STDIN_FILENO) == -1)
		ft_putstr("error stdin heredoc\n", 2);
	close(save_in);
	signal(SIGINT, sighandler);
	return (result);
}
