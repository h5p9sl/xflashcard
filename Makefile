
# Compiler & linker
CFLAGS =
LFLAGS =
CC = gcc

SRCDIR = ./src/
SRC := $(wildcard ${SRCDIR}*.c)
OBJ := ${SRC:.c=.o}
OUT = xflashcard

all: options xflashcard

install: all
	cp ${OUT} /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/xflashcard

options:
	@echo "SRC = ${SRC}"
	@echo "OBJ = ${OBJ}"
	@echo ""

.c.o:
	${CC} ${CFLAGS} -c $< -o $@

xflashcard: ${OBJ}
	${CC} ${LFLAGS} ${OBJ} -o ${OUT}

clean:
	rm ${OBJ}
	rm ${OUT}

