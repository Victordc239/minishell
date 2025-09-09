/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:03:09 by sofernan          #+#    #+#             */
/*   Updated: 2025/07/07 14:20:02 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdarg.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdint.h>
# include <limits.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>

void		free_and_error(char *str, char *message, int exit_code, int std);
void		*ft_calloc(size_t count, size_t size);
void		ft_bzero(void *s, size_t n);

char		*ft_substr(char const *s, unsigned int start, size_t len);
size_t		ft_strcat(char *dst, char const *src);
size_t		ft_strcpy(char *dest, char const *src);
int			ft_strncmp(char const *s1, char const *s2, size_t n);
int			ft_strcmp(char const *s1, char const *s2);
char		*ft_strjoin(char *s1, char const *s2);
char		**ft_split(char const *s, char c);
char		*ft_strchr(char const *s, int c);
char		*ft_strdup(char const *s1);
size_t		ft_strlen(char const *str);
char		*ft_itoa(int n);

void		*ft_memcpy(void *dst, const void *src, size_t n);
void		*ft_memset(void *b, int c, size_t len);

int			ft_atoi(char const *str);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isdigit(int c);

void		ft_putstr(char *s, int std);
void		ft_putchar(char c);

// GNL

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif

char		*free_memory(char **s);
char		*get_first_line(char *file_cont);
char		*remove_first_line(char *file_cont);
char		*ft_strjoin_gnl(char *s1, char *s2);
char		*add_file_cont(int fd, char *file_cont);
char		*get_next_line(int fd);

#endif