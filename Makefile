# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: victor <victor@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/03 11:35:45 by victor            #+#    #+#              #
#    Updated: 2025/10/03 11:39:08 by victor           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#SRCS = \
ft_atoi.c \
ft_bzero.c \
ft_calloc.c \
ft_isalnum.c \
ft_isalpha.c \
ft_isdigit.c \
ft_itoa.c \
ft_memcpy.c \
ft_memset.c \
ft_putchar.c \
ft_putstr.c \
ft_split.c \
ft_strchr.c \
ft_strdup.c \
ft_strjoin.c \
ft_strjoin_gnl.c \
ft_strlen.c \
ft_strncmp.c \
ft_strcmp.c \
ft_substr.c \
get_next_line.c \
ft_strcpy.c \
ft_strcat.c \

#OBJ_DIR = libft_obj
#OBJS = $(SRCS:.c=.o)
#OBJ_FILES = $(addprefix $(OBJ_DIR)/,$(OBJS))

#CC = cc
#RM = rm -f
#CFLAGS = -g -Wall -Wextra -Werror

#NAME = libft.a

#TOTAL_FILES := $(words $(SRCS))
#COUNT = 0

#all: $(OBJ_DIR) $(NAME)

#$(OBJ_DIR):
#	@mkdir -p $(OBJ_DIR)

#$(NAME): $(OBJ_FILES)
#	@ar rcs $(NAME) $(OBJ_FILES)
#	@echo "\033[1;33mLeaving Libft...\033[0m"

#$(OBJ_DIR)/%.o: %.c
#	@mkdir -p $(dir $@)
#	$(eval COUNT=$(shell echo $$(( $(COUNT) + 1 ))))
#	$(eval PERCENT=$(shell echo $$(( $(COUNT) * 100 / $(TOTAL_FILES) ))))
#	@echo "\033[1;32m[ $(PERCENT)%] Building C object $@\033[0m"
#	@$(CC) $(CFLAGS) -c $< -o $@

#clean:
#	@echo "\033[1;33mCleaning all .o files...\033[0m"
#	@$(RM) $(OBJ_FILES) > /dev/null 2>&1
#	@rm -rf $(OBJ_DIR)

#fclean: clean
#	@$(RM) $(NAME) > /dev/null 2>&1

#re: fclean all

#.PHONY: all clean fclean re

SRCS = \
ft_atoi.c \
ft_bzero.c \
ft_calloc.c \
ft_isalnum.c \
ft_isalpha.c \
ft_isdigit.c \
ft_itoa.c \
ft_memcpy.c \
ft_memset.c \
ft_putchar.c \
ft_putstr.c \
ft_split.c \
ft_strchr.c \
ft_strdup.c \
ft_strjoin.c \
ft_strjoin_gnl.c \
ft_strlen.c \
ft_strncmp.c \
ft_strcmp.c \
ft_substr.c \
get_next_line.c \
ft_strcpy.c \
ft_strcat.c

OBJ_DIR = libft_obj
OBJS = $(SRCS:.c=.o)
OBJ_FILES = $(SRCS:%.c=$(OBJ_DIR)/%.o)

CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror

NAME = libft.a

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ_FILES)
	ar rcs $(NAME) $(OBJ_FILES)
	echo "libft: archive created -> $(NAME)"

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ_FILES)
	rm -rf $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
