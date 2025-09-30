/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   25.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:07:18 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/26 20:15:47 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	is_redir(t_redir *redir)
{
	return (redir->type == T_RED_IN || redir->type == T_RED_OUT
		|| redir->type == T_HEREDOC || redir->type == T_RED_APPEND
		|| redir->type == T_RED_INOUT);
}

void	apply_one_redirection(t_minishell *mini, t_redir *redir)
{
	int	fd;

	if (redir->type == T_RED_IN || redir->type == T_HEREDOC)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == T_RED_INOUT)
		fd = open(redir->filename, O_RDWR | O_CREAT, 0644);
	else if (redir->type == T_RED_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		(perror(redir->filename), free_minishell(mini), exit(1));
	if (redir->type == T_RED_IN || redir->type == T_HEREDOC
		|| redir->type == T_RED_INOUT)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			(perror("dup2"), close(fd), free_minishell(mini), exit(1));
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			(perror("dup2"), close(fd), free_minishell(mini), exit(1));
	}
	close(fd);
}

void	apply_redirections(t_minishell *mini)
{
	t_redir	*redir;

	redir = mini->command_list->redirs;
	while (redir)
	{
		if (is_redir(redir) && (!redir->filename
				|| ft_strchr("|'\"<>", redir->filename[0])))
		{
			free_minishell(mini);
			exit_with_error(SYNTAX_ERROR, 2, 2);
		}
		apply_one_redirection(mini, redir);
		redir = redir->next;
	}
}
