/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:38:09 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/06 16:13:37 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

t_minishell	init_minishell(void)
{
	t_minishell	mini;

	ft_bzero(&mini, sizeof(t_minishell));
	mini.env_list = NULL;
	mini.t_list = NULL;
	mini.cmd_list = NULL;
	mini.pipex_data = NULL;
	mini.head = NULL;
	mini.tmp = NULL;
	mini.tokenizer = NULL;
	mini.curr_token = NULL;
	mini.new_token = NULL;
	mini.new_node = NULL;
	mini.current = NULL;
	mini.saved_stdin = -1;
	return (mini);
}

void	update_shlvl(t_minishell *mini)
{
	t_env	*shlvl;
	int		level;
	char	*new_value;

	if (!mini)
		return ;
	shlvl = find_env(mini->env_list, "SHLVL");
	if (!shlvl)
	{
		set_env_var(mini, "SHLVL", "1", 1);
		return ;
	}
	if (!shlvl->value || !is_all_digits(shlvl->value))
		level = 1;
	else
		level = ft_atoi(shlvl->value) + 1;
	if (level > 999)
		level = 1;
	new_value = ft_itoa(level);
	if (!new_value)
		return ;
	if (shlvl->value)
		free(shlvl->value);
	shlvl->value = new_value;
	shlvl->exported = 1;
}

t_env	*create_env_list(char **envp, t_minishell *mini)
{
	int		i;
	char	*equal;
	char	*name;
	char	*value;

	mini->env_list = NULL;
	i = 0;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			name = ft_substr(envp[i], 0, equal - envp[i]);
			value = ft_strdup(equal + 1);
			set_env_var(mini, name, value, 1);
			free(name);
			free(value);
		}
		i++;
	}
	update_shlvl(mini);
	return (mini->env_list);
}

void	exec_subshell_child(t_minishell *parent, char *inner)
{
	t_minishell	child;
	char		**env_arr;
	char		*status_str;

	(signal(SIGINT, SIG_DFL), signal(SIGQUIT, SIG_DFL));
	if (parent->saved_stdin != -1)
		(close(parent->saved_stdin), parent->saved_stdin = -1);
	child = init_minishell();
	env_arr = make_env_array(parent->env_list);
	if (env_arr)
		child.env_list = create_env_list(env_arr, &child);
	status_str = ft_itoa(g_status);
	if (status_str)
		(set_env_var(&child, "?", status_str, 0), free(status_str));
	if (inner)
		process_input(inner, &child);
	else
		process_input("", &child);
	if (child.env_list)
		free_env_list(child.env_list);
	if (env_arr)
		free_str_array(env_arr);
	exit(g_status);
}

int	execute_subshell(t_minishell *parent, char *inner)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		exec_subshell_child(parent, inner);
	if (waitpid(pid, &status, 0) == -1)
		return (perror("waitpid"), 1);
	if (WIFEXITED(status))
		g_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_status = 128 + WTERMSIG(status);
	return (g_status);
}
