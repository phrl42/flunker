SRC_DIR = src/
OBJ_DIR = bin/
INC_DIR = include/

NAME = flunker
# preprocessor (insert all libraries into one big C file) -> compiler (converts C code to assembly code) -> assembler (generates binary files) -> linker (links binary files altogether)
SRCS = main.c init.c
OBJS = $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))

CC = cc
CFLAGS = -static-libgcc -Wall -Wextra -Wpedantic -g -I$(INC_DIR)
LFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)

re: clean all

run:
	./$(NAME)

.PHONY: all clean re run