/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   36ok.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:44:09 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/29 17:36:18 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	is_outer_parenthesized(const char *s)
{
	int		depth;
	size_t	i;
	char	quote;

	depth = 1;
	i = 1;
	quote = 0;
	if (!s || s[0] != '(')
		return (0);
	while (i < ft_strlen(s))
	{
		if ((s[i] == '\'' || s[i] == '"') && !quote)
			quote = s[i];
		else if (s[i] == quote)
			quote = 0;
		else if (!quote)
		{
			if (s[i] == '(')
				depth++;
			else if (s[i] == ')' && --depth == 0)
				break ;
		}
		i++;
	}
	return (depth == 0 && i == ft_strlen(s) - 1);
}

char	*strip_outer_parentheses(char *s, int *removed)
{
	char	*cur;
	char	*tmp;
	int		did_remove;
	int		len;

	if (removed)
		*removed = 0;
	if (!s)
		return (NULL);
	cur = trim_whitespace(s);
	if (!cur)
		return (ft_strdup(""));
	did_remove = 0;
	while (is_outer_parenthesized(cur))
	{
		len = ft_strlen(cur);
		tmp = ft_substr(cur, 1, len - 2);
		if (!tmp)
			return (NULL);
		(free(cur), cur = trim_whitespace(tmp));
		(free(tmp), did_remove = 1);
	}
	if (removed && did_remove)
		*removed = 1;
	return (cur);
}

void	run_group_child(t_minishell *parent, char *inner)
{
	t_minishell	child;
	char		**env_arr;
	char		*status_str;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	child = init_minishell();
	env_arr = env_to_array(parent->env_list);
	if (env_arr)
		child.env_list = create_env_list(env_arr, &child);
	status_str = ft_itoa(g_status);
	if (status_str)
	{
		add_env_node(&child, "?", status_str, 0);
		free(status_str);
	}
	if (inner)
		process_input(inner, &child);
	else
		process_input("", &child);
	if (child.env_list)
		free_env_list(child.env_list);
	if (env_arr)
		ft_freedoom(env_arr);
	exit(g_status);
}

int	execute_group_in_subshell(t_minishell *parent, char *inner)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		run_group_child(parent, inner);
	if (waitpid(pid, &status, 0) == -1)
		return (perror("waitpid"), 1);
	if (WIFEXITED(status))
		g_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_status = 128 + WTERMSIG(status);
	return (g_status);
}

void	process_segment(t_minishell *minishell, char *seg)
{
	char	*inner;
	int		is_group;

	is_group = 0;
	if (!seg || *seg == '\0')
		return ;
	inner = strip_outer_parentheses(seg, &is_group);
	if (is_group)
	{
		if (inner)
			execute_group_in_subshell(minishell, inner);
		else
			execute_group_in_subshell(minishell, "");
		if (inner)
			free(inner);
	}
	else
		process_command(minishell, seg, inner);
}
