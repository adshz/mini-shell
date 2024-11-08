# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: szhong <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/08 12:46:50 by szhong            #+#    #+#              #
#    Updated: 2024/11/08 15:39:45 by szhong           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	minishell
CC			:=	cc
CFLAGS		:=	-Wall -Werror -Wextra -g3
INCLUDE		:= -I./inc -I./libft/inc
SRC_DIR		:=	./src
OBJ_DIR		:=	./obj

SRCS		:=	\
				main.c \

OBJS		:=	$(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(SRCS)))
LIBFT_PATH	:=	./libft
ALL_LIBS	:=	-L$(LIBFT_PATH) -lft
MKFL		:=	--no-print-directory

all: $(NAME)

$(NAME): $(OBJS) libft
	@$(cc) $(CFLAGS) $(OBJS) $(ALL_LIBS) -o $(NAME)
	@echo "[MINISHELL] Build Completed"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@


libft:
	@make $(MKFL) -C ./libft all

clean:
	@make $(MKFL) -C ./libft clean
	@rm -rf $(OBJ_DIR)
	@echo "[MINISHELL] Objects Removed"

fclean: clean
	@make $(MKFL) -C ./libft fclean
	@rm -rf $(NAME)
	@echo "[MINISHELL] Everything Removed"
	@echo "======== PROJECT RESET ========"

re:	fclean all

.PHONY: all clean fclean re
