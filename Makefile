CFILES =	src/main.c \
			src/init_struct.c \
			src/parse_rt_file.c \
			src/parse/parse_ambient.c \
			src/parse/parse_camera.c \
			src/parse/parse_cylinder.c \
			src/parse/parse_light.c \
			src/parse/parse_plane.c \
			src/parse/parse_sphere.c \
			src/parse/parse_utils.c \
			src/render/render_scene.c \
			src/utils/utils.c \

NAME = miniRT
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./include -g
OBJ_DIR = Obj
OFILES = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(CFILES))

LIBFT_URL = https://github.com/nlewicki/LIBFT.git
LIBFT_DIR = lib/libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_INCLUDES = -I $(LIBFT_DIR)

REPO_URL = https://github.com/codam-coding-college/MLX42
REPO_DIR = lib/MLX42
BUILD_DIR = $(REPO_DIR)/build
MLX42_LIB = $(BUILD_DIR)/libmlx42.a
MLX42_INCLUDES = -I $(REPO_DIR)/include

FRAMEWORKS = -L$(shell brew --prefix glfw)/lib -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

all: $(NAME)

$(REPO_DIR):
	@echo "Initializing MLX42 submodule"
	@git submodule update --init --recursive lib/MLX42

$(BUILD_DIR): $(REPO_DIR)
	@echo "Building MLX42 with CMake"
	@cmake -B $(BUILD_DIR) -S $(REPO_DIR)

$(MLX42_LIB): $(BUILD_DIR)
	@echo "Building MLX42"
	@make -C $(BUILD_DIR)

$(NAME): $(MLX42_LIB) $(OFILES) $(LIBFT)
	@echo "Linking $(NAME)"
	@$(CC) $(OFILES) $(LIBFT) $(MLX42_LIB) -o $(NAME) $(FRAMEWORKS)
	clear
	@$(MAKE) loading
	clear

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CC) -c $(CFLAGS) $(LIBFT_INCLUDES) $(MLX42_INCLUDES) $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT_DIR):
	@echo "Initializing Libft submodule"
	@git submodule update --init --recursive lib/libft

$(LIBFT): $(LIBFT_DIR)
	@echo "Building Libft"
	@make -C $(LIBFT_DIR)

loading:
	@for i in {1..42}; do \
		printf '%s' "██"; \
		sleep 0.01; \
	done
	@echo ""

clean:
	@rm -rf $(OBJ_DIR)
	@[ -d $(LIBFT_DIR) ] && make -C $(LIBFT_DIR) clean || true
	@echo "\033[90mclean\033[0m"

fclean: clean
	@rm -f $(NAME)
	@[ -d $(LIBFT_DIR) ] && make -C $(LIBFT_DIR) fclean || true
	@[ -d $(BUILD_DIR) ] && rm -rf $(BUILD_DIR) || true
	@echo "\033[90mfclean\033[0m"

re: fclean all

submodules:
	@git submodule update --init --recursive

.PHONY: all clean fclean re loading submodules
