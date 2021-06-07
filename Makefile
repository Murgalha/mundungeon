comp=gcc
src=src/*.c
incl=-Iinclude
out=./mundungeon
libs=-pthread -lSDL2 -lGL -lm -ldl
std=-std=c99

all:
	@$(comp) -o $(out) $(src) $(incl) $(libs) $(std)

debug:
	@$(comp) -o $(out) $(src) $(incl) $(libs) $(std) -g

run:
	@./$(out)
