/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   37.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:44:16 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/26 19:20:25 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

char	*join_with_marker(char *s1, char *s2)
{
	size_t	l1;
	size_t	l2;
	char	*res;

	if (!s1)
		s1 = ft_strdup("");
	if (!s1)
	{
		free(s2);
		return (NULL);
	}
	if (s1[0] == '\0')
		return (join_free(s1, s2));
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	res = malloc(l1 + 1 + l2 + 1);
	if (!res)
		return (free(s1), free(s2), NULL);
	ft_memcpy(res, s1, l1);
	res[l1] = '\x01';
	ft_memcpy(res + l1 + 1, s2, l2 + 1);
	free(s1);
	free(s2);
	return (res);
}

void	remove_marker_inplace(char *s)
{
	size_t	i;
	size_t	j;

	if (!s)
		return ;
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != '\x01')
			s[j++] = s[i];
		i++;
	}
	s[j] = '\0';
}

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
