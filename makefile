# xDirectories
SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := obj

# Files
EXEC    = $(BIN_DIR)/texteditor
#SRC_ALL = $(wildcard $(SRC_DIR)/*.c)
SRC_ALL = src/userInterface.c src/_main.c src/file.c src/cursor.c
OBJ_ALL = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_ALL))

# Flags/arguments for compilation
FLAGS = -lncurses -c -O2
ARGS  = -I$(SRC_DIR) $(FLAGS) -static

# Compile all .o files to the final executable
$(EXEC): $(OBJ_ALL)
	gcc -o $@ $^ -lncurses -g

# Compile each .c file to corresponding .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	gcc -o $@ $< $(ARGS) -g

.PHONY: run

# make run FILENAME=path/to/file
run: $(EXEC)
	./$(EXEC) $(FILENAME)

clean: cleanobj cleanbin

# Clear obj
cleanobj:
	rm -r $(OBJ_DIR)/*

# Clear bin
cleanbin:
	rm -r $(BIN_DIR)/*

gdb:
	gdb $(EXEC)