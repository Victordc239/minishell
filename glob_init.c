/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:41:47 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/13 15:24:19 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	pattern_has_slash(const char *s)
{
	while (s && *s)
	{
		if (*s == '/')
			return (1);
		s++;
	}
	return (0);
}

void	split_path(const char *pattern, char **dir_out, char **base_out)
{
	int	i;

	*dir_out = NULL;
	*base_out = NULL;
	if (!pattern)
		return ;
	i = (int)ft_strlen(pattern) - 1;
	while (i >= 0 && pattern[i] != '/')
		i--;
	if (i < 0)
	{
		*dir_out = ft_strdup(".");
		*base_out = ft_strdup(pattern);
		return ;
	}
	if (i == 0)
		*dir_out = ft_strdup("/");
	else
		*dir_out = ft_substr(pattern, 0, i);
	*base_out = ft_strdup(pattern + i + 1);
}

int	glob_init(const char *pattern, t_minishell *mini, t_glob_ctx *ctx)
{
	ctx->dir = NULL;
	ctx->pat = NULL;
	ctx->matches = NULL;
	ctx->mcount = 0;
	ctx->mcap = 0;
	ctx->matched_any = 0;
	ctx->d = NULL;
	ctx->allow_dot = 0;
	if (!pattern || !mini)
		return (0);
	if (!ft_strchr(pattern, '*') && !ft_strchr(pattern, '?')
		&& !ft_strchr(pattern, '['))
		return (add_arg_to_cmd(mini, (char *)pattern), 0);
	if (pattern_has_slash(pattern))
		split_path(pattern, &ctx->dir, &ctx->pat);
	else
	{
		ctx->dir = ft_strdup(".");
		ctx->pat = ft_strdup(pattern);
	}
	if (!ctx->dir || !ctx->pat)
		return (free(ctx->dir), free(ctx->pat),
			add_arg_to_cmd(mini, (char *)pattern), 0);
	ctx->allow_dot = (ctx->pat[0] == '.');
	return (1);
}
