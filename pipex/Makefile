# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: carlos-m <carlos-m@student.42madrid.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/21 11:22:09 by carlos-m          #+#    #+#              #
#    Updated: 2024/01/21 11:22:11 by carlos-m         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

SRC = pipex.c

OBJ = $(SRC:.c=.o)

CFLAGS := -Wall -Werror -Wextra
REMOVE = rm -f
CC := cc

all: $(NAME)

$(NAME): $(OBJ) Libft/libft.a
		$(CC) $(OBJ) -o $(NAME) Libft/libft.a

Libft/libft.a:
		make -C Libft/

clean:
		$(REMOVE) $(OBJ)
		make -C Libft/ fclean

fclean: clean
		$(REMOVE) $(NAME)

re: fclean all