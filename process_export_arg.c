/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_export_arg.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:41:25 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/06 16:13:37 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	is_valid_identifier(char const *str)
{
	int	i;

	i = 1;
	if (!str || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

t_env	*find_env(t_env *env_list, char const *name)
{
	while (env_list)
	{
		if (!ft_strcmp(env_list->name, name))
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

void	add_or_update_env(char *arg, t_minishell *mini)
{
	t_env	*var;
	char	*equal;
	char	*name;
	char	*value;

	equal = ft_strchr(arg, '=');
	if (!equal)
		return ;
	name = ft_substr(arg, 0, equal - arg);
	value = ft_strdup(equal + 1);
	var = find_env(mini->env_list, name);
	if (var)
	{
		free(var->value);
		var->value = value;
	}
	else
	{
		set_env_var(mini, name, value, 1);
		free(value);
	}
	free(name);
}

void	mark_as_exported(char *name, t_minishell *mini)
{
	t_env	*var;

	var = find_env(mini->env_list, name);
	if (var)
		var->exported = 1;
	else
		set_env_var(mini, name, NULL, 1);
}

int	process_export_arg(char *arg, t_minishell *mini)
{
	char	*equal;
	char	*name;
	int		error;

	error = 0;
	equal = ft_strchr(arg, '=');
	if (equal)
		name = ft_substr(arg, 0, equal - arg);
	else
		name = ft_strdup(arg);
	if (!is_valid_identifier(name))
	{
		ft_putstr("export: `", 2);
		ft_putstr(arg, 2);
		ft_putstr("': not a valid identifier\n", 2);
		error = 1;
	}
	else if (equal)
		add_or_update_env(arg, mini);
	else
		mark_as_exported(name, mini);
	free(name);
	return (error);
}
