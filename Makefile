# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: szhong <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/08 12:46:50 by szhong            #+#    #+#              #
#    Updated: 2025/02/22 11:58:37 by szhong           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
#Define colour
DF = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
MAGENTA = \033[0;95m
YELLOW = \033[0;93m
CYAN =\033[0;96m
WHITE = \033[0;97m
BOLD = \033[91m
ORANGE = \033[38;5;208m

NAME		:=	minishell
CC			:=	cc
CFLAGS		:=	-Wall -Werror -Wextra -g3
INCLUDE		:=	-I./inc \
                -I./libft/inc \
                -I./src \
                -I./test

SRC_DIR		:=	./src
OBJ_DIR		:=	./obj

SRCS		:=	\
				main.c \
				lexer/lexer.c \
				lexer/lexer_build.c \
				lexer/lexer_classify.c \
				lexer/lexer_init.c \
				lexer/lexer_cleanup.c \
				lexer/lexer_scan.c \
				lexer/lexer_validate.c \
				lexer/tokeniser.c\
				parser/ast/ast_builder.c \
				parser/ast/ast_free.c \
				parser/ast/ast_memory_manager.c \
				builtins/builtins.c \
				builtins/builtins_executor.c \
				builtins/builtin_utils/builtin_utils.c \
				builtins/ft_cd/cd_env_handler.c \
				builtins/ft_cd/cd_executor.c \
				builtins/ft_cd/cd_path_handler.c \
				builtins/ft_cd/cd_utils.c \
				builtins/ft_echo/ft_echo.c \
				builtins/ft_echo/echo_variable_expander.c \
				builtins/ft_echo/echo_state_handler.c \
				builtins/ft_echo/echo_character_handler.c \
				builtins/ft_echo/echo_variable_handler.c \
				builtins/ft_echo/echo_command_handler.c \
				builtins/ft_echo/echo_number_utils.c \
				builtins/ft_env/ft_env.c \
				builtins/ft_env/env_utils.c \
				builtins/ft_exit.c \
				builtins/ft_export/ft_export.c \
				builtins/ft_export/export_utils.c \
				builtins/ft_export/export_value_expansion.c \
				builtins/ft_export/export_parser.c \
				builtins/ft_pwd.c \
				builtins/ft_unset.c \
				hashtable/hashtable_collision.c \
				hashtable/hashtable_create.c \
				hashtable/hashtable_destory.c \
				hashtable/hashtable_env_conversion.c \
				hashtable/hashtable_get.c \
				hashtable/hashtable_insert.c \
				hashtable/hashtable_iterate.c \
				hashtable/hashtable_murmurhash2.c \
				hashtable/hashtable_remove.c \
				hashtable/hashtable_search.c \
				hashtable/hashtable_set.c \
				hashtable/hashtable_size.c \
				utils/cleanup.c \
				utils/command_cleanup.c \
				utils/command_utils.c \
				utils/error_handler.c \
				utils/history.c \
				utils/string_utils.c \
				utils/terminal_cleanup.c

OBJS		:=	$(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(SRCS)))
MKFL		:=	--no-print-directory
LDFLAGS 	+= -lreadline

LIBFT_PATH  := ./libft
LIBFT       := $(LIBFT_PATH)/libft.a
ALL_LIBS    := $(LIBFT) $(LDFLAGS)

all: banner $(NAME)

banner:
	    @echo "$(BOLD) $(GREEN)"
		@echo " ____________________________________________ "
		@echo "|     ____       _____ __  __________    __  | "
		@echo "|    / __ \     / ___// / / / ____/ /   / /  | "
		@echo "|   / / / /_____\__ \/ /_/ / __/ / /   / /   | "
		@echo "|  / /_/ /_____/__/ / __  / /___/ /___/ /___ | "
		@echo "| /_____/     /____/_/ /_/_____/_____/_____/ | "
		@echo "|____________________________________________| "
		@echo "                                              "
		@echo "$(RED)minihell   (.)(.)                       "
		@echo "        \\ /  ()  \\                          "
		@echo "        _ \\ '--' / _                         "
		@echo "       { '-\`\"\"\"\"\"\`-' }                 "
		@echo "        \`\"/      \\\"\`                     "
		@echo "          \\      /                           "
		@echo "         _/  /\\  \\_                         "
		@echo "        {   /  \\   }                          "
		@echo "         \`\"\`    \`\"\`                     "
		@echo "$(DF)"


$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(ALL_LIBS) -o $(NAME)
	@echo "${YELLOW}[MINISHELL] ${GREEN}Build Completed${DF}"

$(LIBFT):
	@make $(MKFL) -C $(LIBFT_PATH) all

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@



clean:
	@make $(MKFL) -C $(LIBFT_PATH) clean
	@rm -rf $(OBJ_DIR)
	@echo "${CYAN}[MINISHELL] Objects Removed${CYAN}"

fclean: clean
	@make $(MKFL) -C $(LIBFT_PATH) fclean
	@rm -rf $(NAME)
	@echo "${CYAN}[MINISHELL] Everything Removed${CYAN}"
	@echo "${BOLD}${ORANGE}======== PROJECT RESET ========${DF}"

re:	fclean all

test: LIBFT
	@echo "${YELLOW}Running some tests${DF}"
	@make $(MKFL) -C tests test || exit 1
	@echo "${GREEN}Tests completed.${DF}"

.PHONY: all clean fclean re
