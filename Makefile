# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joshapir <joshapir@student.42barcelon      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/16 01:00:52 by joshapir          #+#    #+#              #
#    Updated: 2025/03/04 22:08:37 by joshapir         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := so_long

CC := gcc
CFLAGS := -Wall -Wextra -Werror
LFLAGS := -Lmlx_linux/build -lmlx42 -lglfw -ldl -lm -pthread
INCLUDES := -Imlx_linux/include
MLX := ./mlx_linux
LIBMLX := $(MLX)/build/libmlx42.a

SRCS := main.c check_map.c get_next_line.c get_next_line_utils.c \
        ft_itoa.c movements.c map_utils.c map_components.c \
        map_free_utils.c collectibles_utils.c movements_utils.c \
        init_sprites.c render_map.c allocs.c cleanup.c ft_printf.c
OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(NAME):  $(OBJS) $(LIBMLX)
	$(CC) $(CFLAGS) $(OBJS) $(LIBMLX) $(LFLAGS) -o $(NAME)

$(LIBMLX):
	@cmake -B $(MLX)/build $(MLX)  # Ensure CMake generates the build system
	@cmake --build $(MLX)/build -j4  # Build MLX42

%.o: %.c so_long.h ft_printf.h Makefile
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	@if [ -d $(MLX)/build ]; then cmake --build $(MLX)/build --target clean; fi

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

