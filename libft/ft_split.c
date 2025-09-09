/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:34:46 by sofernan          #+#    #+#             */
/*   Updated: 2025/07/07 13:54:17 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

static int	ft_wordlen(char const *s, char c)
{
	int	len;

	len = 0;
	while (*s && *s != c)
	{
		len++;
		s++;
	}
	return (len);
}

static int	ft_countwords(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if ((s[i] == c && s[i + 1] != c && s[i + 1] != '\0')
			|| (s[i] != c && i == 0))
		{
			count++;
			i++;
		}
		else
			i++;
	}
	return (count);
}

static void	*free_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	int		i;
	int		count;

	if (!s)
		return (NULL);
	count = ft_countwords(s, c);
	strs = malloc(sizeof(char *) * (count + 1));
	if (!strs)
		return (NULL);
	strs[count] = NULL;
	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			strs[i] = ft_substr(s, 0, ft_wordlen(s, c));
			if (!strs[i++])
				return (free_strs(strs));
			s = s + ft_wordlen(s, c);
		}
		else
			s++;
	}
	return (strs);
}
/*
int main(void)
{
    char const *str = "      split       this for   me  !       ";
    char **words = ft_split(str, ' ');

	if (words)
    {
        int i = 0;
        while (words[i])
        {
            printf("%s\n", words[i]);
            i++;
        }
    }
	return (0);
}
*/
