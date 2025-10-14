/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:41:47 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/14 16:53:56 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	str_has_slash(const char *s)
{
	while (s && *s)
	{
		if (*s == '/')
			return (1);
		s++;
	}
	return (0);
}

void	split_path(const char *str, char **dir_out, char **base_out)
{
	int	i;

	*dir_out = NULL;
	*base_out = NULL;
	if (!str)
		return ;
	i = (int)ft_strlen(str) - 1;
	while (i >= 0 && str[i] != '/')
		i--;
	if (i < 0)
	{
		*dir_out = ft_strdup(".");
		*base_out = ft_strdup(str);
		return ;
	}
	if (i == 0)
		*dir_out = ft_strdup("/");
	else
		*dir_out = ft_substr(str, 0, i);
	*base_out = ft_strdup(str + i + 1);
}

int	init_glob(const char *str, t_minishell *mini, t_glob_ctx *ctx)
{
	ctx->dir = NULL;
	ctx->pat = NULL;
	ctx->matches = NULL;
	ctx->mcount = 0;
	ctx->mcap = 0;
	ctx->matched_any = 0;
	ctx->d = NULL;
	ctx->allow_dot = 0;
	if (!str || !mini)
		return (0);
	if (!ft_strchr(str, '*') && !ft_strchr(str, '?')
		&& !ft_strchr(str, '['))
		return (add_arg_to_cmd(mini, (char *)str), 0);
	if (str_has_slash(str))
		split_path(str, &ctx->dir, &ctx->pat);
	else
	{
		ctx->dir = ft_strdup(".");
		ctx->pat = ft_strdup(str);
	}
	if (!ctx->dir || !ctx->pat)
		return (free(ctx->dir), free(ctx->pat),
			add_arg_to_cmd(mini, (char *)str), 0);
	ctx->allow_dot = (ctx->pat[0] == '.');
	return (1);
}
