# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/15 17:56:20 by musajid           #+#    #+#              #
#    Updated: 2025/10/15 18:49:43 by musajid          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I .

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

LIBFTDIR = libft
LIBFTNAME = $(LIBFTDIR)/libft.a

LDFLAGS = -lreadline

all: $(NAME)

$(NAME): $(LIBFTNAME) $(OBJ)
	$(CC) $(OBJ) $(LIBFTNAME) $(LDFLAGS) -o $(NAME)

$(LIBFTNAME):
	@make -C $(LIBFTDIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make clean -C $(LIBFTDIR)
	@rm -f $(OBJ)

fclean: clean
	@make fclean -C $(LIBFTDIR)
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJ)
