/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_dir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:32:13 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/14 16:36:51 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	ensure_capacity(char ***arr, size_t *count, size_t *cap)
{
	size_t		newcap;
	char		**newarr;
	size_t		i;

	if (*cap == 0)
	{
		*cap = 8;
		*arr = malloc(sizeof(char *) * (*cap));
		if (!*arr)
			return (0);
		return (1);
	}
	if (*count + 1 > *cap)
	{
		newcap = (*cap * 2);
		newarr = malloc(sizeof(char *) * newcap);
		if (!newarr)
			return (0);
		i = 0;
		while (i++ < *count)
			newarr[i - 1] = (*arr)[i - 1];
		(free(*arr), *arr = newarr);
		*cap = newcap;
	}
	return (1);
}

int	insert_sorted(char ***arr, size_t *count, size_t *cap, char *s)
{
	size_t	pos;
	size_t	j;

	pos = 0;
	if (!ensure_capacity(arr, count, cap))
		return (0);
	while (pos < *count && ft_strcmp((*arr)[pos], s) < 0)
		pos++;
	j = *count;
	while (j > pos)
	{
		(*arr)[j] = (*arr)[j - 1];
		j--;
	}
	(*arr)[pos] = s;
	(*count)++;
	return (1);
}

int	process_and_insert(t_glob_ctx *ctx, const char *name)
{
	char	*tmp;
	char	*full;

	tmp = NULL;
	full = NULL;
	if (ft_strcmp(ctx->dir, ".") == 0)
		full = ft_strdup(name);
	else if (ft_strcmp(ctx->dir, "/") == 0)
		full = ft_strjoin("/", name);
	else
	{
		tmp = ft_strjoin(ctx->dir, "/");
		if (!tmp)
			return (0);
		full = ft_strjoin(tmp, name);
		free(tmp);
	}
	if (!full)
		return (0);
	if (!insert_sorted(&ctx->matches, &ctx->mcount, &ctx->mcap, full))
		return (free(full), 0);
	ctx->matched_any = 1;
	return (1);
}

void	free_matches_recursive(t_glob_ctx *ctx, size_t idx)
{
	if (!ctx->matches)
		return ;
	if (idx >= ctx->mcount)
	{
		free(ctx->matches);
		ctx->matches = NULL;
		return ;
	}
	free(ctx->matches[idx]);
	free_matches_recursive(ctx, idx + 1);
}

int	process_dir(t_glob_ctx *ctx, char *str, t_minishell *mini)
{
	struct dirent	*entry;
	const char		*name;

	entry = readdir(ctx->d);
	if (!entry)
		return (1);
	name = entry->d_name;
	if (!name || name[0] == '\0')
		return (process_dir(ctx, str, mini));
	if (!ctx->allow_dot && name[0] == '.')
		return (process_dir(ctx, str, mini));
	if (match_glob(ctx->pat, name))
	{
		if (!process_and_insert(ctx, name))
		{
			free_matches_recursive(ctx, 0);
			closedir(ctx->d);
			free(ctx->dir);
			free(ctx->pat);
			add_arg_to_cmd(mini, str);
			return (0);
		}
	}
	return (process_dir(ctx, str, mini));
}
