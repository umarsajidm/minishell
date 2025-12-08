# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/29 17:14:37 by achowdhu          #+#    #+#              #
#    Updated: 2025/12/08 13:45:42 by musajid          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ========================================
#           Project Information
# ========================================

NAME        := minishell

# Compiler and Flags
CC          := cc
CFLAGS      := -Wall -Wextra -Werror -g
#CFLAGS		:= -ggdb3 -fsanitize=address -fsanitize=leak -fsanitize=undefined

# Directories
SRC_DIR     := src
OBJ_DIR     := obj
INC_DIR     := include
LIBFT_DIR   := libft

# Source and Object Files
SRCS        := $(shell find $(SRC_DIR) -name "*.c")
OBJS        := $(addprefix $(OBJ_DIR)/,$(SRCS:$(SRC_DIR)/%.c=%.o))

# Library
LIBFT       := $(LIBFT_DIR)/libft.a
READLINE    := -lreadline

# Includes
INC         := -I$(INC_DIR) -I$(LIBFT_DIR)/include

# ========================================
#               Colors
# ========================================

GREEN       := \033[0;32m
YELLOW      := \033[0;33m
RESET       := \033[0m

# ========================================
#               Rules
# ========================================

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "$(GREEN)[Linking]$(RESET) $(NAME)"
	@$(CC) $(CFLAGS) $(INC) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)
	@echo "$(GREEN)✔ $(NAME) built successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(GREEN)[Compiling]$(RESET) $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT):
	@echo "$(YELLOW)[Building]$(RESET) libft"
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@echo "$(YELLOW)[Cleaning]$(RESET) object files"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(YELLOW)[Removing]$(RESET) $(NAME)"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

# ========================================
#              Special Rules
# ========================================

.PHONY: all clean fclean re
.SECONDARY:
