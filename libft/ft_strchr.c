/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 13:04:10 by sofernan          #+#    #+#             */
/*   Updated: 2025/07/07 13:54:17 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
#include "libft.h"

char	*ft_strchr(char const *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
		{
			return ((char *)s);
		}
		s++;
	}
	if ((char)c == '\0')
	{
		return ((char *)s);
	}
	return (NULL);
}
/*
int main(void)
{
    char const *str = "Hola hola";
    int ch = '\0';
    char *result = ft_strchr(str, ch);
    if (result != NULL)
	{
        printf("El carácter '%c' en la posición %ld\n", ch, result - str);
    }
	else
	{
        printf("El carácter '%c' no se encuentra\n", ch);
    }
    return (0);
}
*/
