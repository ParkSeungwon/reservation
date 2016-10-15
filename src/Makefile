CFLAG = -g -c -std=c++11 -fmax-errors=1 -pthread
INCLUDE = -I../
CC = g++
OBJ = $(patsubst %.cc, ../OBJ/%.o, $(wildcard *.cc))
MAIN_OBJ = $(patsubst %.cpp, ../OBJ/%.x, $(wildcard *.cpp))

all : $(OBJ) $(MAIN_OBJ)

../OBJ/%.o : %.cc 
	$(CC) $(GTKCFLAG) $(INCLUDE) $(CFLAG) $< -o $@

../OBJ/%.x : %.cpp
	$(CC) $(GTKCFLAG) $(INCLUDE) $(CFLAG) $< -o $@
