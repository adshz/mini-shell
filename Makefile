# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: szhong <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/08 12:46:50 by szhong            #+#    #+#              #
#    Updated: 2024/12/19 17:59:23 by szhong           ###   ########.fr        #
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
				executor/env_executor/env_array_manager.c \
				utils/mem_tracker.c \
				utils/astnode_cleanup.c \
				parser/redirection/heredoc_chain_handler_utils.c \
				parser/redirection/redirection_init_utils.c \
				main.c \
				core/core.c \
				core/ft_signals.c \
				core/init.c \
				core/init_env.c \
				core/shell_modes.c \
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
				executor/ast_executor/ast_executor.c \
				executor/ast_executor/node_handler.c \
				executor/ast_executor/redirection_handler.c \
				executor/command_check/command_path_check.c \
				executor/command_check/command_path_check_utils.c \
				executor/command_check/command_path_resolver.c \
				executor/command_check/command_path_searcher.c \
				executor/command_executor/command_error_handler.c \
				executor/command_executor/command_executor.c \
				executor/command_executor/command_process_handler.c \
				executor/command_executor/command_executor_signals.c \
				executor/command_executor/command_external_signals.c \
				executor/command_expander/path_handler.c \
				executor/command_expander/variable_expander.c \
				executor/command_expander/command_name_expander.c \
				executor/command_expander/args_array_handler.c \
				executor/command_expander/args_array_handler_utils.c \
				executor/command_expander/args_array_helper.c \
				executor/command_expander/args_array_expander_utils.c \
				executor/command_expander/args_array_split_utils.c \
				executor/command_expander/command_name_expander_args_build.c \
				executor/command_expander/command_name_expand_array_build_utils.c \
				executor/command_expander/command_name_expander_prefix_handler.c \
				executor/command_expander/command_name_expander_var_expansion.c \
				executor/env_executor/env_array_creator.c \
				executor/env_executor/env_array_builder.c \
				executor/env_executor/env_string_builder.c \
				executor/env_executor/env_array_string_helper.c \
				executor/env_executor/env_variable_handler.c \
				executor/env_executor/env_hashmap_handler.c \
				executor/env_executor/env_item_processor.c \
				executor/env_executor/env_item_processor_utils.c \
				executor/env_executor/env_mem_utils.c \
				executor/pipe_handler/pipe_child.c \
				executor/pipe_handler/pipe_executor.c \
				executor/pipe_handler/pipe_process_handler.c \
				executor/pipe_handler/pipe_setup.c \
				executor/pipe_handler/pipe_status_handler.c \
				executor/pipe_handler/pipe_utils.c \
				executor/redirections_executor/heredoc_processor.c \
				executor/redirections_executor/heredoc_processor_utils.c \
				executor/redirections_executor/redirection_executor.c \
				executor/redirections_executor/redirection_process_handler.c \
				executor/redirections_executor/redirection_signal_handler.c \
				executor/redirections_executor/redirection_filename_expander.c \
				executor/redirections_executor/redirection_filename_utils.c \
				executor/redirections_executor/redirection_input_handler.c \
				executor/redirections_executor/redirection_output_handler.c \
				executor/redirections_executor/redirection_processor.c \
				executor/redirections_executor/redirection_fd_handler.c \
				executor/redirections_executor/redirection_node_handler.c \
				executor/redirections_executor/redirection_utils.c \
				expander/expand_variables/command_expander.c \
				expander/expand_variables/command_expander_var_processor.c \
				expander/expand_variables/simple_variable_expander.c \
				expander/expand_variables/tilde_expander.c \
				expander/expander_utils.c \
				expander/quote_handling.c \
				expander/special_expansions.c \
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
				lexer/token_length_calculator.c \
				lexer/token_state_transitions.c \
				lexer/token_type_handler.c \
				lexer/token_utils.c \
				lexer/token_extractor.c \
				lexer/tokeniser.c \
				parser/parser.c \
				parser/ast/ast_free.c \
				parser/ast/ast_builder.c \
				parser/command/command_node.c \
				parser/command/command_init.c \
				parser/command/command_init_utils.c \
				parser/command/command_parser.c \
				parser/command/command_utils.c \
				parser/expression_parser/parser_expression.c \
				parser/variable_expansion/parser_variable_expander.c \
				parser/pipeline/pipeline_parser.c \
				parser/redirection/heredoc_handler.c \
				parser/redirection/heredoc_chain_handler.c \
				parser/redirection/heredoc_token_handler.c \
				parser/redirection/redirection_handler.c \
				parser/redirection/redirection_heredoc_handler.c \
				parser/redirection/redirection_init.c \
				parser/redirection/redirection_parser.c \
				parser/redirection/redirection_utils.c \
				parser/redirection/redirection_token_handler.c \
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
