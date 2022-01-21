# C compiler options
CC := c99
CFLAG = -g -O0 -Wall
EXEC_FORMAT := elf
SRC_DIR := src
OBJ := obj
INCLUDE_DIR := include
TEST_DIR := test

# OS env
RM := rm
MKDIR := mkdir

# DEPS = $(patsubst %.c, %.o, $(wildcard *.c))
DEPS = $(patsubst %.c, $(OBJ)/%.o, $(wildcard *.c))

.SECONDARY: 
	$(OBJ)/%.o

build: $(OBJ) main.$(EXEC_FORMAT)

test1: $(OBJ) test1.$(EXEC_FORMAT) 
	./test1.$(EXEC_FORMAT) < test/test.in

clean:
	$(RM) -rf $(OBJ)
	$(RM) *.$(EXEC_FORMAT)

%.$(EXEC_FORMAT): $(OBJ)/cmap.o $(OBJ)/%.o
	$(CC) $(CFLAG) -o $@ $^

$(OBJ)/%.o: %.c
	$(CC) $(CFLAG) -o $@ $^ -c -I./

$(OBJ)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAG) -o $@ $^ -c -I./

$(OBJ):
	$(MKDIR) $@
