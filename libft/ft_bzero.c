/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 13:34:22 by sofernan          #+#    #+#             */
/*   Updated: 2025/07/07 13:57:14 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}
/*
int main(void)
{
    char buffer[20] = "Hola, 42";

    printf("Antes de ft_bzero: %s\n", buffer);
    ft_bzero(buffer, sizeof(buffer));
    printf("Después de ft_bzero: %s\n", buffer);
    return (0);
}
*/
