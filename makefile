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

build: $(BIN) $(BIN)/libcmap.so
	$(CP) cmap.h $(BIN)/
	
main: main.$(EXEC_FORMAT)
	./main.$(EXEC_FORMAT)

test1: test1.$(EXEC_FORMAT) 
	./test1.$(EXEC_FORMAT) < test/test.in

test2: test2.$(EXEC_FORMAT)
	./test2.$(EXEC_FORMAT) < test/test.in

clean:
	$(FIND) ./ -type f -name *.$(EXEC_FORMAT) -exec $(RM) {} \;

$(BIN)/libcmap.so: cmap.c
	$(CC) -shared -fPIC -o $@ $^

%.$(EXEC_FORMAT): cmap.o %.o
	$(CC) $(CFLAG) -o $@ $^

%.o: %.c
	$(CC) -c $(CFLAG) -o $@ $^ -I./

%.o: $(TEST_DIR)/%.c
	$(CC) -c $(CFLAG) -o $@ $^ -I./

$(BIN):
	$(MKDIR) $@
