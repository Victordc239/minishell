/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:05:31 by sofernan          #+#    #+#             */
/*   Updated: 2025/07/07 13:38:08 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
#include "libft.h"

char	*ft_strjoin(char *s1, char const *s2)
{
	char	*result;
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;
	size_t	j;

	s1_len = 0;
	s2_len = 0;
	i = -1;
	j = -1;
	if (!s1 || !s2)
		return (NULL);
	while (s1[s1_len])
		s1_len++;
	while (s2[s2_len])
		s2_len++;
	result = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (s1[++i])
		result[i] = s1[i];
	while (s2[++j])
		result[i + j] = s2[j];
	result[i + j] = '\0';
	return (result);
}
/*
int main(void)
{
    char s1[] = "Hola, ";
    char s2[] = "42";
    char *result = ft_strjoin(s1, s2);
	
	printf("Resultado de la concatenación: %s\n", result);
    return (0);
}
*/
