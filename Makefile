# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/26 22:48:03 by sangcpar          #+#    #+#              #
#    Updated: 2021/10/16 14:55:01 by sangcpar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project file
NAME = minishell

# Project builds and dirs
SRCS = cmd_env.c\
		cmd_export.c\
		ft_split_1.c\
		ft_split_2.c\
		ft_split_utils.c\
		main.c\
		operator.c\
		parse.c\
		pipe.c\
		run_cmd.c\
		signal.c\
		termios.c\
		utils.c

SRCDIR = ./
SRCNAMES = $(shell ls $(SRCDIR) | grep -E ".+\.c")
SRC = $(addprefix $(SRCDIR), $(SRCNAMES))
INC = ./minishell.h
BUILDDIR = ./build/
BUILDOBJS = $(addprefix $(BUILDDIR)/, $(SRCS:.c=.o))

# Libft builds and dirs
LIBDIR = ./libft/
LIBFT = ./libft/libft.a
LIBINC = ./libft/

# Optimization and Compiler flags and commands
CC = gcc 
CFLAGS = -Wall -Werror -Wextra
READ = -lreadline -L/Users/sangcpar/.brew/opt/readline/lib -I/Users/sangcpar/.brew/opt/readline/include
READ2 = -I/Users/sangcpar/.brew/opt/readline/include

# Debugging flags
DEBUG = -g

# Main rule
all: lib $(NAME)

# Objects rule
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(INC)
	$(CC) $(CFLAGS) $(READ2) -I $(LIBINC) -I $(INC) -c $< -o $@

# Libft rule
lib:
	make -C $(LIBDIR)

# Project file rule
$(NAME): $(BUILDDIR) $(BUILDOBJS)
	$(CC) -L$(LIBDIR) -lft $(BUILDOBJS) $(READ) -o $(NAME)

# Object dir rule
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Cleaning up the build files
clean:
	rm -rf $(BUILDDIR)
	make -C $(LIBDIR) clean

# Getting rid of the project file
fclean: clean
	rm -rf $(NAME)
	make -C $(LIBDIR) fclean

# Do both of the above
re: fclean all

# Just in case those files exist in the root dir
.PHONY: all fclean clean re

# gcc *.c libft/libft.a -lreadline -L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include
# gcc *.c libft/libft.a -lreadline -L$(HOME)/.brew/opt/readline/lib -I$(HOME)/.brew/opt/readline/include