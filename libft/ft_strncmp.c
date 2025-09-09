/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 13:24:29 by sofernan          #+#    #+#             */
/*   Updated: 2025/07/07 13:54:17 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
#include "libft.h"

int	ft_strncmp(char const *s1, char const *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	while (i < n && *s1 != '\0' && *s1 == *s2)
	{
		s1++;
		s2++;
		i++;
	}
	if (i < n)
		return ((unsigned char)*s1 - (unsigned char)*s2);
	else
		return (0);
}
/*
int main(void) 
{
    char *str1 = "Hola 42";
	char *str2 = "Hola";
	int result = ft_strncmp(str1, str2, 5);

	if (result < 0)
        printf("'%s' es menor que '%s'\n", str1, str2);
    else if (result > 0)
        printf("'%s' es mayor que '%s'\n", str1, str2);
    else
        printf("'%s' es igual a '%s'\n", str1, str2);
	return (0);
}
*/
