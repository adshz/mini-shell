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
CC			:=	gcc
CFLAGS		:=	-Wall -Werror -Wextra -g3
INCLUDE		:=	-I./inc \
                -I./libft/inc \
                -I./src \
                -I./test

# OS detection
UNAME_S := $(shell uname -s)

# Readline paths based on OS
ifeq ($(UNAME_S),Darwin)
    # macOS
    READLINE_DIR := $(shell brew --prefix readline)
    INCLUDE += -I$(READLINE_DIR)/include
    LDFLAGS = -L$(READLINE_DIR)/lib -lreadline
else
    # Linux
    LDFLAGS = -lreadline
endif

SRC_DIR		:=	./src
OBJ_DIR		:=	./obj

SRCS		:=	\
				core/core.c \
				core/init.c \
				signals/signal_handlers.c \
				signals/signal_status.c \
				main.c \
				lexer/lexer.c \
				lexer/lexer_build.c \
				lexer/lexer_classify.c \
				lexer/lexer_init.c \
				lexer/lexer_cleanup.c \
				lexer/lexer_scan.c \
				lexer/lexer_validate.c \
				lexer/lexer_utils.c \
				parser/ast/ast_node_builder.c \
				parser/ast/ast_memory_manager.c \
				parser/command/command_node_builder.c \
				parser/command/command_text_processor.c \
				parser/parser.c \
				parser/parser_utils/parser_state.c \
				parser/parser_expression/atomic_expression_handler.c \
				parser/parser_expression/expression_parser.c \
				parser/parser_redirection/io_node_manager.c \
				parser/parser_redirection/redirection_processor.c \
				expander/expander.c \
				expander/expander_utils/array_utils.c \
				expander/expander_utils/globber_utils.c \
				expander/expander_utils/expander_string_utils.c \
				expander/submodules/globber.c \
				expander/submodules/quote_handler.c \
				expander/submodules/quote_stripper.c \
				expander/submodules/token_expander.c \
				expander/submodules/variable_expander.c \
				expander/submodules/wildcard_matcher.c \
				expander/submodules/word_splitter.c \
				builtins/ft_cd.c \
				builtins/ft_echo.c \
				builtins/ft_env.c \
				builtins/ft_exit.c \
				builtins/ft_export.c \
				builtins/ft_pwd.c \
				builtins/ft_unset.c \
				executor/executor.c \
				executor/ast_executor/ast_executor.c \
				executor/ast_executor/logic_node_handler.c \
				executor/builtins_executor/builtin_executor.c \
				executor/command_executor/command_executor.c \
				executor/command_executor/external_cmd.c \
				executor/heredoc/heredoc_expander.c \
				executor/heredoc/heredoc_handler.c \
				executor/heredoc/heredoc_processor.c \
				executor/heredoc/heredoc_utils.c \
				executor/path_resolver/path_resolver.c \
				executor/path_resolver/path_validator.c \
				executor/pipe_executor/pipe_child.c \
				executor/pipe_executor/pipe_executor.c \
				executor/executor_redirection/input_redirection.c \
				executor/executor_redirection/output_redirection.c \
				executor/executor_redirection/redirection_handler.c \
				executor/executor_utils/error_printer.c \
				executor/executor_utils/exec_exit_status.c \
				hashtable/hashtable_collision.c \
				hashtable/hashtable_create_item.c \
				hashtable/hashtable_create.c \
				hashtable/hashtable_destory.c \
				hashtable/hashtable_env_conversion.c \
				hashtable/hashtable_get_item.c \
				hashtable/hashtable_free_item.c \
				hashtable/hashtable_insert.c \
				hashtable/hashtable_iterate.c \
				hashtable/hashtable_murmurhash2.c \
				hashtable/hashtable_remove.c \
				hashtable/hashtable_replace_value.c \
				hashtable/hashtable_search.c \
				hashtable/hashtable_set.c \
				hashtable/hashtable_size.c \
				memory_collector/memory_collector.c \
				memory_collector/memory_deleter.c \
				utils/cleanup_shell.c \
				utils/string_utils.c \
				utils/exit_handler.c


OBJS		:=	$(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(SRCS)))
MKFL		:=	--no-print-directory
#LDFLAGS 	+= -lreadline

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
