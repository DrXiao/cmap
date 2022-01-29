# C compiler options
CC := c99
CFLAG = -g -O0 -Wall
EXEC_FORMAT := elf
SRC_DIR := src
INCLUDE_DIR := include
TEST_DIR := test
BIN := bin

# OS env
RM := rm
MKDIR := mkdir
FIND := find
CP := cp
EXEC := exec

# Valgrind args
VALGRIND_ARGS := --leak-check=full --show-leak-kinds=all --track-origins=yes -s

build: $(BIN) $(BIN)/libcmap.so
	$(CP) cmap.h $(BIN)/

check:
	$(FIND) ./ -type f -name "*.$(EXEC_FORMAT)" -$(EXEC) valgrind $(VALGRIND_ARGS) {} < test/test.in \;

clean:
	$(FIND) ./ -type f -name "*.$(EXEC_FORMAT)" -$(EXEC) $(RM) {} \;

$(BIN)/libcmap.so: cmap.c
	$(CC) -shared -fPIC -o $@ $^

%.$(EXEC_FORMAT): cmap.o %.o
	$(CC) $(CFLAG) -o $@ $^
	./$@ < test/test.in

%.o: %.c
	$(CC) -c $(CFLAG) -o $@ $^ -I./

%.o: $(TEST_DIR)/%.c
	$(CC) -c $(CFLAG) -o $@ $^ -I./

$(BIN):
	$(MKDIR) $@
