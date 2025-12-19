# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: musajid <musajid@hive.student.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/29 17:14:37 by achowdhu          #+#    #+#              #
#    Updated: 2025/12/18 13:02:33 by jtarvain         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ========================================
#           Project Information
# ========================================

NAME        := minishell

# Compiler and Flags
CC          := cc
CFLAGS		:= -Wall -Wextra -Werror -g
#CFLAGS		:= -ggdb3 -fsanitize=address -fsanitize=leak -fsanitize=undefined

# Directories
SRC_DIR     := src
OBJ_DIR     := obj
INC_DIR     := include
LIBFT_DIR   := libft

# Source and Object Files
SRCS        := src/built-ins/cd_utils.c \
             src/built-ins/ft_cd.c \
             src/built-ins/ft_echo.c \
             src/built-ins/ft_env.c \
             src/built-ins/ft_exit.c \
             src/built-ins/ft_exit2.c \
             src/built-ins/ft_exit_utils.c \
             src/built-ins/ft_export.c \
             src/built-ins/ft_export_utils.c \
             src/built-ins/ft_pwd.c \
             src/built-ins/ft_unset.c \
             src/built-ins/is_builtin.c \
             src/built-ins/print_export_utils.c \
             src/built-ins/run_builtin.c \
             src/cleanup/shell_cleanup.c \
             src/environment/env.c \
             src/environment/envp.c \
             src/executor/execution_1.c \
             src/executor/execution_2.c \
             src/executor/freenerr.c \
             src/executor/main_pipeline_utils.c \
             src/executor/pipeline.c \
             src/executor/pipeline_1.c \
             src/executor/pipeline_error.c \
             src/executor/pipeline_exit_utilities.c \
             src/executor/pipeline_utils.c \
             src/executor/preexec_clean.c \
             src/executor/validate_command.c \
             src/main.c \
             src/parser/expansion.c \
             src/parser/exp_utils.c \
             src/parser/field_splitting.c \
             src/parser/parse.c \
             src/parser/parser_utils.c \
             src/parser/redir.c \
             src/parser/token_handler.c \
             src/repl/read.c \
             src/repl/repl.c \
             src/shell/shell.c \
             src/shell/shell_cleanup.c \
             src/signals/signals.c \
             src/signals/s_handlers.c \
             src/tokenizer/handlers.c \
             src/tokenizer/token.c \
             src/tokenizer/token_utils.c \
             src/utils/arena_malloc.c \
             src/utils/arena_utils.c \
             src/utils/utils.c
OBJS        := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Library
LIBFT       := $(LIBFT_DIR)/libft.a
READLINE    := -lreadline

# Includes
INC         := -I$(INC_DIR) -I$(LIBFT_DIR)/include

vpath %.c $(SRC_DIR) $(SRC_DIR)/built-ins $(SRC_DIR)/cleanup $(SRC_DIR)/environment $(SRC_DIR)/executor $(SRC_DIR)/parser $(SRC_DIR)/repl $(SRC_DIR)/shell $(SRC_DIR)/signals $(SRC_DIR)/tokenizer $(SRC_DIR)/utils

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

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
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
	@rm -rf $(OBJ_DIR)/*
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(YELLOW)[Removing]$(RESET) $(NAME)"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

# ========================================
#              Special Rules
# ========================================

.PHONY: all clean fclean re bonus
.SECONDARY:
