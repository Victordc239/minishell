/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_parent.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:36:45 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/13 14:46:10 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	parse_exit_code(const char *s, unsigned char *out_code, int i, int neg)
{
	unsigned int	r;

	if (!s)
		return (0);
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			neg = 1;
		i++;
	}
	if (!s[i])
		return (0);
	r = 0;
	while (s[i])
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (0);
		r = (r * 10 + (s[i] - '0')) & 0xFF;
		i++;
	}
	if (neg)
		*out_code = (unsigned char)((256 - (r & 0xFF)) & 0xFF);
	else
		*out_code = (unsigned char)(r & 0xFF);
	return (1);
}

int	ft_exit(t_minishell *mini)
{
	unsigned char	code;

	if (!mini->cmd_list->argv[1])
	{
		free_minishell(mini);
		exit((unsigned char)g_status);
	}
	if (mini->cmd_list->argv[2])
	{
		ft_putstr("exit: too many arguments\n", 2);
		g_status = 1;
		return (1);
	}
	if (!parse_exit_code(mini->cmd_list->argv[1], &code, 0, 0))
	{
		ft_putstr("exit: ", 2);
		ft_putstr(mini->cmd_list->argv[1], 2);
		ft_putstr(": numeric argument required\n", 2);
		free_minishell(mini);
		exit(2);
	}
	free_minishell(mini);
	exit((int)code);
}

void	delete_env_var(char const *name, t_minishell *mini)
{
	t_env	*curr;
	t_env	*prev;

	curr = mini->env_list;
	prev = NULL;
	while (curr)
	{
		if (!ft_strcmp(curr->name, name))
		{
			if (prev)
				prev->next = curr->next;
			else
				mini->env_list = curr->next;
			free(curr->name);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	ft_unset(t_minishell *mini)
{
	int		i;

	i = 1;
	while (mini->cmd_list->argv[i])
	{
		delete_env_var(mini->cmd_list->argv[i], mini);
		i++;
	}
}

void	exec_builtin_parent(t_minishell *mini)
{
	if (!ft_strcmp(mini->cmd_list->argv[0], "exit"))
		ft_exit(mini);
	else if (!ft_strcmp(mini->cmd_list->argv[0], "cd"))
		ft_cd(mini, 0, 0);
	else if (!ft_strcmp(mini->cmd_list->argv[0], "unset"))
		ft_unset(mini);
	else if (!ft_strcmp(mini->cmd_list->argv[0], "export"))
		ft_export(mini);
}
