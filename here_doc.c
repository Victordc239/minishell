/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:40:52 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/02 17:28:08 by sofernan         ###   ########.fr       */
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

int	write_heredoc_line(int fd, char *line, t_minishell *mini,
					t_token_quote quote)
{
	char	*expanded;

	if (quote == Q_NONE)
	{
		expanded = expand_env_in_str(line, mini);
		if (!expanded)
			expanded = ft_strdup("");
		write(fd, expanded, ft_strlen(expanded));
		(write(fd, "\n", 1), free(expanded));
	}
	else
		(write(fd, line, ft_strlen(line)), write(fd, "\n", 1));
	return (0);
}

int	process_heredoc(int fd, const char *limiter, t_minishell *mini, t_token_quote quote)
{
	char	*line;
	size_t	len;

	while (1)
	{
		if (g_status == 130)
			return (130);
		(write(1, "> ", 2), line = get_next_line(0));
		if (g_status == 130)
			return (free(line), 130);
		if (!line)
		{
			ft_putstr("heredoc delimited by EOF\n", 2);
			return (0);
		}
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (ft_strcmp(line, limiter) == 0)
			return (free(line), 0);
		(write_heredoc_line(fd, line, mini, quote), free(line));
	}
	return (0);
}

void	sighandler(int signal)
{
	if (signal == SIGINT)
	{
		g_status = 128 + SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	here_doc(const char *limiter, const char *filename, t_minishell *mini, t_token_quote quote)
{
	int	fd;
	int	save_in;
	int	result;

	save_in = dup(STDIN_FILENO);
	signal(SIGINT, heredoc_signal);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		dup2(save_in, STDIN_FILENO);
		close(save_in);
		return (-1);
	}
	if (!limiter || !*limiter)
		exit_with_error(SYNTAX_ERROR, 1, 2);
	result = process_heredoc(fd, limiter, mini, quote);
	close(fd);
	if (dup2(save_in, STDIN_FILENO) == -1)
		ft_putstr("error stdin heredoc\n", 2);
	close(save_in);
	signal(SIGINT, sighandler);
	return (result);
}
