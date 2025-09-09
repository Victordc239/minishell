/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:09:45 by sofernan          #+#    #+#             */
/*   Updated: 2025/07/07 13:54:17 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned int	i;

	i = 0;
	if (src == NULL && dest == NULL)
		return (NULL);
	while (i < n)
	{
		((char *)dest)[i] = ((char const *)src)[i];
		i++;
	}
	return (dest);
}
/*
int main(void)
{
    char *src = "Hola 42";
    char dst[20];
    ft_memcpy(dst, src, 7);
	printf("dest: %s\n", dst);
    return (0);
}
*/
