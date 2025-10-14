/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_matches.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:41:51 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/13 17:16:55 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	add_arg_to_cmd(t_minishell *mini, char *arg)
{
	int		count;
	int		i;
	char	**new_argv;

	count = 0;
	i = 0;
	if (mini->curr->argv)
	{
		while (mini->curr->argv[count])
			count++;
	}
	new_argv = malloc(sizeof(char *) * (count + 2));
	if (!new_argv)
		exit_with_error("Error malloc argv\n", 1, 2);
	while (i < count)
	{
		new_argv[i] = mini->curr->argv[i];
		i++;
	}
	new_argv[count] = ft_strdup(arg);
	new_argv[count + 1] = NULL;
	free(mini->curr->argv);
	mini->curr->argv = new_argv;
}

int	try_open_dir(t_glob_ctx *ctx, char *str, t_minishell *mini)
{
	ctx->d = opendir(ctx->dir);
	if (!ctx->d)
	{
		free(ctx->dir);
		free(ctx->pat);
		add_arg_to_cmd(mini, str);
		return (0);
	}
	return (1);
}

void	add_matches(t_minishell *mini, t_glob_ctx *ctx, size_t idx)
{
	if (idx >= ctx->mcount)
		return ;
	add_arg_to_cmd(mini, ctx->matches[idx]);
	free(ctx->matches[idx]);
	add_matches(mini, ctx, idx + 1);
}

int	expand_matches(char *str, t_minishell *mini)
{
	t_glob_ctx	ctx;
	int			ok;

	if (!init_glob(str, mini, &ctx))
		return (1);
	if (!try_open_dir(&ctx, str, mini))
		return (1);
	ok = process_dir(&ctx, str, mini);
	if (!ok)
		return (1);
	closedir(ctx.d);
	if (!ctx.matched_any)
	{
		free(ctx.dir);
		free(ctx.pat);
		if (ctx.matches)
			free_matches_recursive(&ctx, 0);
		add_arg_to_cmd(mini, str);
		return (1);
	}
	add_matches(mini, &ctx, 0);
	free(ctx.matches);
	free(ctx.dir);
	free(ctx.pat);
	return (1);
}
