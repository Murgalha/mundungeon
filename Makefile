CXX ?= g++
SRCDIR=src/
INCLUDEDIR=include/
WARNFLAGS=-Wall -Wextra -fpermissive
LIBS=-lGL -lSDL2 -lm -lfreetype
STD=-std=c++14

WRKDIR=build/
OBJDIR := ${WRKDIR}obj/
HEADERFILES := $(wildcard ${INCLUDEDIR}*.h)
SRCFILES := $(wildcard ${SRCDIR}*.cpp)
OBJFILES := ${addprefix ${OBJDIR}, ${notdir ${SRCFILES:.cpp=.o}}}

# EXECUTABLE STUFF
BIN=mundungeon
BINDIR := ${WRKDIR}bin/
BINFILE := ${BINDIR}${BIN}

all: prepare ${BINFILE}

${OBJDIR}%.o: ${SRCDIR}%.cpp ${HEADERFILES}
	$(CXX) -c $< ${WARNFLAGS} -I${INCLUDEDIR} -o $@ ${STD} -g -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-4 -pthread

${BINFILE}: ${OBJFILES}
	$(CXX) $^ ${WARNFLAGS} -I${INCLUDEDIR} -o $@ ${STD} ${LIBS} -g -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-4 -pthread

run:
	@./${BINFILE}

prepare:
	@if [ ! -d "${WRKDIR}" ]; then mkdir ${WRKDIR}; fi
	@if [ ! -d "${OBJDIR}" ]; then mkdir ${OBJDIR}; fi
	@if [ ! -d "${BINDIR}" ]; then mkdir ${BINDIR}; fi

clear:
	rm -rf ${WRKDIR}
