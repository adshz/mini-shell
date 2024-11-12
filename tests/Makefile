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

NAME	:= test_runner
CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror -g
RM		:= rm -rf

# Path to Prjects
PROJECT_ROOT	:=	..
PROJECT_INC		:= -I$(PROJECT_ROOT)/inc -I$(PROJECT_ROOT)/libft/inc
PROJECT_SRC		:=  $(PROJECT_ROOT)/src

# Libft paths
LIBFT_PATH	:= $(PROJECT_ROOT)/libft
LIBFT		:= $(LIBFT_PATH)/libft.a

# Test Library Paths
TEST_LIB_PATH	:=	ft_test
TEST_LIB		:=	$(TEST_LIB_PATH)/libtest.a
TEST_INC		:=	-I$(TEST_LIB_PATH)/inc


# Source files
SRC_DIR			:=	./
TEST_CASES_DIR	:=	test_cases

# Project source files needed for tests
# Find all project source files (excluding main.c)
#PROJECT_SRCS	:=	$(shell find $(PROJECT_SRCS) -type f -name "*.c" ! -name "main.c" -printf "%P\n")
PROJECT_SRCS	:=	builtins/pwd.c

# Find all test source files
TEST_SRCS		:=	test_main.c \
					$(TEST_CASES_DIR)/test_builtins.c
#					$(shell find $(TEST_CASES_DIR) -type f -name "*.c" -printf "%P\n")

# Object files
OBJ_DIR			:=	obj
PROJECT_OBJS	:=	$(addprefix $(OBJ_DIR)/, $(PROJECT_SRCS:.c=.o))
TEST_OBJS		:=	$(addprefix $(OBJ_DIR)/, $(TEST_SRCS:.c=.o))

VPATH			:=	$(PROJECT_SRC):test_cases:.

# Colors for output
GREEN			:=	\033[0;32m
YELLOW			:=	\033[0;33m
RESET			:=	\033[0m

MKFLAGS			:= --no-print-directory

all: print_files $(NAME)

# Debug target to print found files
print_files:
	@echo "$(YELLOW)Project source files:$(RESET)"
	@echo "$(PROJECT_SRCS)"
	@echo "$(YELLOW)Test source files:$(RESET)"
	@echo "$(TEST_SRCS)"



$(NAME): $(TEST_LIB) $(PROJECT_OBJS) $(TEST_OBJS)
	@$(CC) $(CFLAGS) $(PROJECT_OBJS) $(TEST_OBJS) \
		-L$(LIBFT_PATH) -lft \
		-L$(TEST_LIB_PATH) -ltest \
		-o $(NAME)
	@echo "$(GREEN)Test runner built successfully!$(RESET)"

$(TEST_LIB):
	@make $(MKFLAGS) -C $(TEST_LIB_PATH)

# Compile project source files
$(OBJ_DIR)/builtins/%.o: $(PROJECT_SRC)/builtins/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(PROJECT_INC) -c $< -o $@
	@echo "$(GREEN)Compiling: $<$(RESET)"

# Compile test source files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(PROJECT_INC) $(TEST_INC) -c $< -o $@
	@echo "$(GREEN)Compiling: $<$(RESET)"

clean:
	@$(RM) $(OBJ_DIR)
	@make $(MKFLAGS) -C $(TEST_LIB_PATH) clean

fclean: clean
	@$(RM) $(NAME)
	@make $(MKFLAGS) -C $(TEST_LIB_PATH) fclean

re: fclean all

run: all
	@./$(NAME)

.PHONY: all clean fclean re run print_files

# Debug target
debug:
	@echo "PROJECT_ROOT: $(PROJECT_ROOT)"
	@echo "PROJECT_SRC: $(PROJECT_SRC)"
	@echo "PROJECT_OBJS: $(PROJECT_OBJS)"
	@echo "TEST_OBJS: $(TEST_OBJS)"
	@echo "LIBFT: $(LIBFT)"
	@echo "TEST_LIB: $(TEST_LIB)"
