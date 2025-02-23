# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: raljaber <raljaber@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/12 16:15:44 by raljaber          #+#    #+#              #
#    Updated: 2024/09/12 16:15:44 by raljaber         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CFLAGS = -Wall -Wextra -Werror

SRC = exit.c routines.c utils.c monitor.c init.c philo.c parsing.c forks.c

OBJ = ${SRC:.c=.o}

all: $(NAME)

$(NAME): $(OBJ)
	${CC} ${CFLAGS} ${OBJ} -o ${NAME} -lpthread

clean:
	rm -f ${OBJ}

fclean: clean
	rm -f philo

re: fclean all
