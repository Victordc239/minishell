# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: victor <victor@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/07 14:20:26 by sofernan          #+#    #+#              #
#    Updated: 2025/10/03 12:58:05 by victor           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror				# podemos quitar el -g3 porque es para debuguear
LIBFT = libft
LIBFT_DIR = libft/
LIBFT_LIB = $(LIBFT_DIR)libft.a
LDFLAGS = -lreadline						# enlaza con la libreria readline

OBJ_DIR = obj
SRCS = todo.c

OBJS = $(SRCS:.c=.o)
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(OBJS))		# antepone obj/ a cada .o, p.e. obj/todo.o
TEMP_DIR = .temp							# carpeta temporal que podemos borrar porque no sirve y no usamos

NAME = minishell
INCLUDES = -I. -I includes					# podemos dejarlo asi INCLUDES = -I. porque el -I includes es para una carpeta includes que nosotros no tenemos

TOTAL_FILES := $(words $(SRCS))				# podemos quitarlo porque es para que salga el porcenataje seguen ejecutamos
COUNT = 0								# podemos quitarlo porque es para que salga el porcenataje seguen ejecutamos

all: $(LIBFT_LIB) $(NAME) $(TEMP_DIR)

$(LIBFT_LIB):
	@echo "\033[1;33mCompiling libft...\033[0m"
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

#lo de arriba el echo es para que salga en color amarillo: Compiling libft...
# y lo de --no-print-directory es para qeu no salga un mensaje de que entra en la carpeta de libft: make[1]: se entra en el directorio '/home/victor/Escritorio/42_Madrid/cursus/mini/libft'

$(NAME): $(OBJ_FILES) $(LIBFT_LIB)
	@ $(CC) $(CFLAGS) $(OBJ_FILES) -o $(NAME) $(LIBFT_LIB) $(LDFLAGS)

$(TEMP_DIR):
	@mkdir -p $(TEMP_DIR)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@if [ "$(COUNT)" -eq "0" ]; then echo "\033[1;33mCompiling Mini...\033[0m"; fi
	$(eval COUNT=$(shell echo $$(( $(COUNT) + 1 ))))
	$(eval PERCENT=$(shell echo $$(( $(COUNT) * 100 / $(TOTAL_FILES) ))))
	@echo "\033[1;32m[ $(PERCENT)%] Building C object $@\033[0m"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "\033[1;33mCleaning all .o files...\033[0m"
	@rm -rf $(OBJ_DIR) > /dev/null 2>&1
	@make clean -C $(LIBFT_DIR) > /dev/null 2>&1

fclean: clean
	@rm -rf $(NAME) $(TEMP_DIR) > /dev/null 2>&1
	@make fclean -C $(LIBFT_DIR) > /dev/null 2>&1

# en fclean en la linea @rm -rf $(NAME) $(TEMP_DIR) > /dev/null 2>&1 podemos dejarlo: @rm -rf $(NAME) > /dev/null 2>&1 porque no usamos esa carpeta para nada

re: fclean all

.PHONY: all clean fclean re
