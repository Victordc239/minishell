/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 12:44:43 by sofernan          #+#    #+#             */
/*   Updated: 2025/07/07 13:57:34 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
#include "libft.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}
/*
int	main(void)
{
    char test_char = '1';
	if (ft_isdigit(test_char))
	{
        printf("Es un dígito\n");
    }
	else
	{
        printf("No es un dígito\n");
	}
    return (0);
}
*/
