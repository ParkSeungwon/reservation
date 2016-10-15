CFLAG = -g -c -std=c++11 -fmax-errors=1
INCLUDE = -I../ -I../src
GTKCFLAG = $(shell pkg-config gtkmm-3.0 --cflags) #-I$(ROOT)
CC = g++
OBJ = $(patsubst %.cc, ../OBJ/%.o, $(wildcard *.cc))
MAIN_OBJ = $(patsubst %.cpp, ../OBJ/%.x, $(wildcard *.cpp))

all : $(OBJ) $(MAIN_OBJ)

../OBJ/%.o : %.cc 
	$(CC) $(GTKCFLAG) $(INCLUDE) $(CFLAG) $< -o $@

../OBJ/%.x : %.cpp
	$(CC) $(GTKCFLAG) $(INCLUDE) $(CFLAG) $< -o $@

