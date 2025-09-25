# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/23 17:17:45 by frey-gal          #+#    #+#              #
#    Updated: 2025/09/25 17:35:59 by joshapir         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ================================== VARS ==================================== #

NAME		= cub3d
CC			= gcc
CFLAGS		= #-Wall -Wextra -Werror -fsanitize=address -O3 -ffast-math -pipe \
			  -march=native -flto #-g

SRC_DIR		= sources
SOURCES		= $(wildcard sources/*.c) #$(addprefix $(SRC_DIR)/, main.c)

OBJ_DIR		= objects
OBJECTS		= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

INCLUDES 	= -Ilibft -I./MLX42/include -I.

HEADER		= cub3d.h

MLX42_DIR	= libraries/MLX42
MLX42		= build/libmlx42.a
MLXFLAGS	= -ldl -lglfw -pthread -lm

# ================================== LIBFT =================================== #

LIBFT_DIR	= libraries/libft

LIBFT		= $(LIBFT_DIR)/libft.a

LIB_HDR		= $(LIBFT_DIR)/libft.h

LIB_SRC		= $(addprefix $(LIBFT_DIR)/ft_, atoi.c bzero.c calloc.c isalnum.c \
			  isalpha.c isascii.c isdigit.c isprint.c itoa.c \
			  lstadd_back_bonus.c lstadd_front_bonus.c lstclear_bonus.c \
			  lstdelone_bonus.c lstiter_bonus.c lstlast_bonus.c lstmap_bonus.c \
			  lstnew_bonus.c lstsize_bonus.c memchr.c memcmp.c memcpy.c \
			  memmove.c memset.c printf.c putchar_fd.c putendl_fd.c \
			  putnbr_fd.c putstr_fd.c split.c strchr.c strdup.c striteri.c \
			  strjoin.c strlcat.c strlcpy.c strlen.c strmapi.c strncmp.c \
			  strnstr.c strrchr.c strtrim.c substr.c tolower.c toupper.c) \
			  $(addprefix $(LIBFT_DIR)/, get_next_line_bonus.c \
			  get_next_line_utils_bonus.c print_base.c print_char.c \
			  print_ptr.c print_str.c print_uns.c)

LIB_DEP = $(LIB_SRC) $(LIB_HDR) $(LIBFT_DIR)/Makefile

# ================================== RULES =================================== #

all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT) $(MLX42)
	@echo "\n==> Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) $(MLX42) $(MLXFLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER) $(LIB_HDR) Makefile | $(OBJ_DIR)
	@echo " -> Compiling hollow3d (HUMANITY RESTORED) $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT): $(LIB_DEP)
	@echo "\n==> Building Libft..."
	@$(MAKE) -C $(LIBFT_DIR)

$(MLX42):
	@echo "\n==> Building MLX..."
	@cmake $(MLX42_DIR) -B build
	@cmake --build build -j4

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	@echo "\n==> Cleaning project..."
	@$(MAKE) -s clean -C $(LIBFT_DIR)
	@rm -f $(wildcard $(OBJ_DIR)/*.o)
	@rm -rf build

fclean: clean
	@echo "\n==> Full clean..."
	@$(MAKE) -s fclean -C $(LIBFT_DIR)
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
