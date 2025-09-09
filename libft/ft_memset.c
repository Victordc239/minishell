/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:35:33 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/03/19 12:35:33 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*str;
	size_t			i;

	i = 0;
	str = (unsigned char *)s;
	while (i < n)
	{
		((unsigned char *)str)[i] = (unsigned char)c;
		i++;
	}
	return (s);
}
/*
int	main(void)
{
	char	str[] = "HOLA ADIOS";
	int	rem = '*';
	size_t	n = 4;
	printf("Resultado: %s\n", ft_memset(str, rem, n));
	return(0);
}*/