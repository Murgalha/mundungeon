CC ?= gcc
SRCDIR=src/
INCLUDEDIR=include/
WARNFLAGS=-Wall -Wextra -Werror
LIBS=-lGL -lSDL2 -lm
STD=-std=c99

WRKDIR=build/
OBJDIR := ${WRKDIR}obj/
HEADERFILES := $(wildcard ${INCLUDEDIR}*.h)
SRCFILES := $(wildcard ${SRCDIR}*.c)
OBJFILES := ${addprefix ${OBJDIR}, ${notdir ${SRCFILES:.c=.o}}}

# EXECUTABLE STUFF
BIN=mundungeon
BINDIR := ${WRKDIR}bin/
BINFILE := ${BINDIR}${BIN}

all: prepare ${BINFILE}

${OBJDIR}%.o: ${SRCDIR}%.c ${HEADERFILES}
	$(CC) -c $< ${WARNFLAGS} -I${INCLUDEDIR} -o $@ ${STD}

${BINFILE}: ${OBJFILES}
	$(CC) $^ ${WARNFLAGS} -I${INCLUDEDIR} -o $@ ${STD} ${LIBS}

run:
	@./${BINFILE}

prepare:
	@if [ ! -d "${WRKDIR}" ]; then mkdir ${WRKDIR}; fi
	@if [ ! -d "${OBJDIR}" ]; then mkdir ${OBJDIR}; fi
	@if [ ! -d "${BINDIR}" ]; then mkdir ${BINDIR}; fi

clear:
	rm -rf ${WRKDIR}
