# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: szhong <szhong@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/12 22:14:48 by szhong            #+#    #+#              #
#    Updated: 2024/11/12 22:25:18 by szhong           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors and formatting
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
RESET = \033[0m
BOLD = \033[1m

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I../inc -I../external/Unity/src -I../libft/inc -I.

# Source files
SRC_DIR = ../src
TEST_DIR = .
UNITY_DIR = ../external/Unity/src
LIBFT = ../libft/libft.a

SRC_FILES = \
			$(SRC_DIR)/hashtable/ft_hashtable.c \
			$(SRC_DIR)/init/init.c \
			$(SRC_DIR)/lexer/tokeniser.c \
			$(SRC_DIR)/parser/parser.c \
			$(SRC_DIR)/executor/executor.c \
			$(SRC_DIR)/executor/pipe_handler.c \
			$(SRC_DIR)/executor/redirections.c \
			$(SRC_DIR)/utils/cleanup.c \
			$(SRC_DIR)/builtins/builtins.c \
			$(SRC_DIR)/utils/string_utils.c \
			$(SRC_DIR)/builtins/ft_pwd.c \
			$(SRC_DIR)/builtins/ft_echo.c \
			$(SRC_DIR)/builtins/ft_cd.c \
			$(SRC_DIR)/builtins/ft_export.c \
			$(SRC_DIR)/builtins/ft_unset.c \
			$(SRC_DIR)/builtins/ft_env.c \
			$(SRC_DIR)/builtins/ft_exit.c

# Unity source
UNITY_SRC = $(UNITY_DIR)/unity.c

# Test executables
TEST_INIT = test_init_runner
TEST_HASH = test_hash_runner
TEST_TOKEN = test_token_runner
TEST_PARSE = test_parse_runner
TEST_EXEC = test_executor_runner
TEST_BUILTINS = test_builtins_runner

$(TEST_INIT): test_init_unity.c $(SRC_FILES) $(UNITY_SRC)
	@echo "$(CYAN)Building $(BOLD)$@$(RESET)..."
	@$(CC) $(CFLAGS) $(UNITY_SRC) $< $(SRC_FILES) $(LIBFT) -o $@

$(TEST_HASH): test_hashtable_unity.c $(SRC_FILES) $(UNITY_SRC)
	@echo "$(CYAN)Building $(BOLD)$@$(RESET)..."
	@$(CC) $(CFLAGS) $(UNITY_SRC) $< $(SRC_FILES) $(LIBFT) -o $@

$(TEST_TOKEN): test_tokenizer_unity.c $(SRC_FILES) $(UNITY_SRC)
	@echo "$(CYAN)Building $(BOLD)$@$(RESET)..."
	@$(CC) $(CFLAGS) $(UNITY_SRC) $< $(SRC_FILES) $(LIBFT) -o $@

$(TEST_PARSE): test_parser_unity.c $(SRC_FILES) $(UNITY_SRC)
	@echo "$(CYAN)Building $(BOLD)$@$(RESET)..."
	@$(CC) $(CFLAGS) $(UNITY_SRC) $< $(SRC_FILES) $(LIBFT) -o $@

$(TEST_EXEC): test_executor_unity.c $(SRC_FILES) $(UNITY_SRC)
	@echo "$(CYAN)Building $(BOLD)$@$(RESET)..."
	@$(CC) $(CFLAGS) $(UNITY_SRC) $< $(SRC_FILES) $(LIBFT) -o $@

$(TEST_BUILTINS): test_builtins_unity.c $(SRC_FILES) $(UNITY_SRC)
	@echo "$(CYAN)Building $(BOLD)$@$(RESET)..."
	@$(CC) $(CFLAGS) $(UNITY_SRC) $< $(SRC_FILES) $(LIBFT) -o $@

clean:
	@echo "$(YELLOW)Cleaning up test executables...$(RESET)"
	rm -f $(TEST_INIT) $(TEST_HASH) $(TEST_TOKEN) $(TEST_PARSE) $(TEST_EXEC) $(TEST_BUILTINS)

fclean: clean

re: fclean all

all: $(TEST_INIT) $(TEST_HASH) $(TEST_TOKEN) $(TEST_PARSE) $(TEST_EXEC) $(TEST_BUILTINS)

test: all
	@echo "\n$(MAGENTA)$(BOLD)========== Running Tests ===========$(RESET)\n"
	@echo "$(BLUE)$(BOLD)Running Init Tests:$(RESET)"
	@./$(TEST_INIT)
	@echo "\n$(BLUE)$(BOLD)Running Hashtable Tests:$(RESET)"
	@./$(TEST_HASH)
	@echo "\n$(BLUE)$(BOLD)Running Parser Tests:$(RESET)"
	@./$(TEST_PARSE)
	@echo "\n$(BLUE)$(BOLD)Running Executor Tests:$(RESET)"
	@./$(TEST_EXEC)
	@echo "\n$(BLUE)$(BOLD)Running Builtins Tests:$(RESET)"
	@./$(TEST_BUILTINS)
	@if [ $$? -eq 0 ]; then \
		echo "$(GREEN)$(BOLD)All tests completed successfully!$(RESET)"; \
	else \
		echo "$(RED)$(BOLD)Some tests failed!$(RESET)"; \
	fi

.PHONY: all clean fclean re test
