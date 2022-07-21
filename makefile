SRC=src
INC=inc
LIB=lib
OBJ=obj
BIN=bin
TARGET = wordcombo
CC = gcc
CFLAGS = -O3
ARGS = 101
RM = rm -rf
INCLUDE = -I $(INC) -I $(LIB)
SOURCES=$(wildcard $(SRC)/*.c)
OBJECTS=$(subst $(SRC),$(OBJ),$(patsubst %.c, %.o, $(SOURCES)))


all: $(TARGET)

print:
	@echo  Source Files $(SOURCES)
	@echo Object Files $(OBJECTS)
	@echo Include Command $(INCLUDE)
	@echo Compiler flags $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) $(INCLUDE) $(OBJECTS) $(CFLAGS) -o $(TARGET)

# Pattern to compile and assemble source files into object files
$(OBJ)/%.o : $(SRC)/%.c
	$(CC) $(INCLUDE) $(CFLAGS) $< -c -o $@

clean:
	rm -f $(OBJECTS)
