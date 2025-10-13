/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_glob.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:41:14 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/13 18:15:00 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

const char	*init_class(const char *p, char c, int *neg, int *matched)
{
	*neg = 0;
	*matched = 0;
	if (!p || *p != '[')
		return (NULL);
	p++;
	if (*p == '!' || *p == '^')
	{
		*neg = 1;
		p++;
	}
	if (*p == ']')
	{
		if (c == ']')
			*matched = 1;
		p++;
	}
	return (p);
}

const char	*process_class_content(const char *p, char c, int *matched)
{
	char	a;
	char	b;
	char	tmp;

	if (!p || *p == '\0' || *p == ']')
		return (p);
	if (p[0] && p[1] == '-' && p[2] && p[2] != ']')
	{
		a = p[0];
		b = p[2];
		if (a > b)
		{
			tmp = a;
			a = b;
			b = tmp;
		}
		if ((unsigned char)c >= (unsigned char)a
			&& (unsigned char)c <= (unsigned char)b)
			*matched = 1;
		return (process_class_content(p + 3, c, matched));
	}
	if (c == *p)
		*matched = 1;
	return (process_class_content(p + 1, c, matched));
}

int	match_class(const char **pp, char c)
{
	const char	*p;
	int			negate;
	int			matched;

	p = init_class(*pp, c, &negate, &matched);
	if (!p)
		return (-1);
	p = process_class_content(p, c, &matched);
	if (*p != ']')
		return (-1);
	p++;
	*pp = p;
	if (negate)
		return (!matched);
	return (matched);
}

int	match_question(const char *p, const char *str)
{
	if (*str == '\0')
		return (0);
	return (match_glob(p + 1, str + 1));
}

int	match_glob(const char *p, const char *str)
{
	if (*p == '\0')
		return (*str == '\0');
	if (*p == '*')
	{
		while (*p == '*')
			p++;
		if (*p == '\0')
			return (1);
		while (*str != '\0' && !match_glob(p, str))
			str++;
		return (match_glob(p, str));
	}
	if (*p == '?')
		return (match_question(p, str));
	if (*p == '[')
	{
		if (*str == '\0')
			return (0);
		if (match_class(&p, *str) <= 0)
			return (0);
		return (match_glob(p, str + 1));
	}
	if (*str == '\0' || *p != *str)
		return (0);
	return (match_glob(p + 1, str + 1));
}
