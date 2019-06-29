
# Compiler & linker
CFLAGS =
LFLAGS =
CC = gcc

SRCDIR = ./src/
SRC := $(wildcard ${SRCDIR}*.c)
OBJ := ${SRC:.c=.o}

all: options xflashcard

options:
	@echo "SRC 	= ${SRC}"
	@echo "OBJ	= ${OBJ}"

.c.o:
	${CC} ${CFLAGS} -c $< -o $@

xflashcard: ${OBJ}
	${CC} ${LFLAGS} ${OBJ} -o xflashcard

clean:
	rm ${OBJ}
	rm xflashcard

