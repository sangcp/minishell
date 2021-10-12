# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/26 22:48:03 by sangcpar          #+#    #+#              #
#    Updated: 2021/09/28 19:43:33 by sangcpar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project file
NAME = minishell

# Project builds and dirs
SRCDIR = ./
SRCNAMES = $(shell ls $(SRCDIR) | grep -E ".+\.c")
SRC = $(addprefix $(SRCDIR), $(SRCNAMES))
INC = ./
BUILDDIR = ./build/
BUILDOBJS = $(addprefix $(BUILDDIR), $(SRCNAMES:.c=.o))

# Libft builds and dirs
LIBDIR = ./libft/
LIBFT = ./libft/libft.a
LIBINC = ./libft/

# Optimization and Compiler flags and commands
CC = gcc 
CFLAGS = -Wall -Werror -Wextra
READ = -lreadline -L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include

# Debugging flags
DEBUG = -g

# Main rule
all: $(BUILDDIR) $(LIBFT) $(NAME)

# Object dir rule
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Objects rule
$(BUILDDIR)%.o:$(SRCDIR)%.c
	$(CC) $(CFLAGS) -I$(LIBINC) -I$(INC) -o $@ -c $<

# Project file rule
$(NAME): $(BUILDOBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(BUILDOBJS) $(LIBFT) $(READ)

# Libft rule
$(LIBFT):
	make -C $(LIBDIR)

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