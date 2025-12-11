# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/23 17:17:45 by frey-gal          #+#    #+#              #
#    Updated: 2025/12/11 22:13:02 by joshapir         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ================================== VARS ==================================== #

NAME        = cub3d
CC          = gcc
CFLAGS      = -fsanitize=address -g -O3 -ffast-math -pipe \
              -march=native -flto -funroll-loops

# ================================ MANDATORY ================================= #

SRC_DIR     = sources
SOURCES     = $(addprefix $(SRC_DIR)/, \
                camera.c \
                elements.c \
                flood_fill.c \
                helpers.c \
                init_utils.c \
                main.c \
                map_size.c \
                map_utils.c \
                mem_manager.c \
                movement.c \
                parsing.c \
                raycasting.c \
                raycasting_helpers.c \
                raycasting_walls.c \
                textures_and_colors.c)

OBJ_DIR     = objects
OBJECTS     = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# ================================== BONUS =================================== #

SRC_DIR_B   = sources_bonus
SOURCES_B   = $(addprefix $(SRC_DIR_B)/, \
                camera.c \
                draw_enemy_helpers.c \
                draw_enemy_utils.c \
                elements.c \
                enemies.c \
                enemy_utils.c \
                flood_fill.c \
                fps.c \
                helpers.c \
                init_utils.c \
                main.c \
                map_size.c \
                map_utils.c \
                mem_manager.c \
                minimap.c \
                minimap_draw_helpers.c \
                minimap_draw_utils.c \
                minimap_utils.c \
                movement.c \
                parsing.c \
                raycasting.c \
                raycasting_doors.c \
                raycasting_doors_helpers.c \
                raycasting_floor_ceiling.c \
                raycasting_helpers.c \
                raycasting_walls.c \
                sword_animation.c \
                textures_and_colors.c)

OBJ_DIR_B   = objects_bonus
OBJECTS_B   = $(patsubst $(SRC_DIR_B)/%.c,$(OBJ_DIR_B)/%.o,$(SOURCES_B))

# ================================= COMMON =================================== #

INCLUDES    = -Ilibft -I./MLX42/include -I.
HEADER      = cub3d.h

MLX42_DIR   = libraries/MLX42
MLX42       = build/libmlx42.a
MLXFLAGS    = -ldl -lglfw -pthread -lm

# ================================== LIBFT =================================== #

LIBFT_DIR   = libraries/libft
LIBFT       = $(LIBFT_DIR)/libft.a
LIB_HDR     = $(LIBFT_DIR)/libft.h
LIB_SRC     = $(addprefix $(LIBFT_DIR)/ft_, atoi.c bzero.c calloc.c isalnum.c \
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
LIB_DEP     = $(LIB_SRC) $(LIB_HDR) $(LIBFT_DIR)/Makefile

# ================================== RULES =================================== #

all: $(NAME)

bonus: .bonus

.bonus: $(OBJECTS_B) $(LIBFT) $(MLX42)
	@echo "\n==> Linking $(NAME) (BONUS)..."
	@$(CC) $(CFLAGS) $(OBJECTS_B) $(LIBFT) $(MLX42) $(MLXFLAGS) -o $(NAME)
	@touch .bonus
	@rm -f .mandatory

$(NAME): .mandatory

.mandatory: $(OBJECTS) $(LIBFT) $(MLX42)
	@echo "\n==> Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) $(MLX42) $(MLXFLAGS) -o $(NAME)
	@touch .mandatory
	@rm -f .bonus

# Mandatory compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER) $(LIB_HDR) Makefile | $(OBJ_DIR)
	@echo " -> Compiling hollow3d (HUMANITY RESTORED) $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Bonus compilation
$(OBJ_DIR_B)/%.o: $(SRC_DIR_B)/%.c $(HEADER) $(LIB_HDR) Makefile | $(OBJ_DIR_B)
	@echo " -> Compiling hollow3d BONUS (PRAISE THE SUN) $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT): $(LIB_DEP)
	@echo "\n==> Building Libft..."
	@$(MAKE) -C $(LIBFT_DIR)

$(MLX42):
	@echo "\n==> Building MLX..."
	@cmake $(MLX42_DIR) -B build
	@cmake --build build -j4

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR_B):
	@mkdir -p $(OBJ_DIR_B)

clean:
	@echo "\n==> Cleaning project..."
	@$(MAKE) -s clean -C $(LIBFT_DIR)
	@rm -rf $(OBJ_DIR) $(OBJ_DIR_B)
	@rm -rf build
	@rm -f .mandatory .bonus

fclean: clean
	@echo "\n==> Full clean..."
	@$(MAKE) -s fclean -C $(LIBFT_DIR)
	@rm -f $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re
