/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:02:22 by sofernan          #+#    #+#             */
/*   Updated: 2025/07/07 13:58:56 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
#include "libft.h"

static int	ft_size(int n)
{
	int		size;
	long	nb;

	nb = n;
	size = 1;
	if (nb < 0)
	{
		size++;
		nb = -nb;
	}
	while (nb >= 10)
	{
		size++;
		nb = nb / 10;
	}
	return (size);
}

char	*ft_itoa(int nb)
{
	char	*str;
	long	nbr;
	int		i;

	i = 0;
	str = (char *)malloc(sizeof(char) * ft_size(nb) + 1);
	if (!str)
		return (NULL);
	if (nb < 0)
		str[0] = '-';
	nbr = nb;
	if (nbr < 0)
		nbr = -nbr;
	str[ft_size(nb) - i] = '\0';
	i++;
	while (nbr >= 10)
	{
		str[ft_size(nb) - i] = (nbr % 10) + 48;
		i++;
		nbr = nbr / 10;
	}
	str[ft_size(nb) - i] = (nbr % 10) + 48;
	i++;
	return (str);
}
/*
int main(void)
{
    int number = -1234;
    char *str = ft_itoa(number);
    printf("Número convertido a cadena: %s\n", str);
    return (0);
}
*/
