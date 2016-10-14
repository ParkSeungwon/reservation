all : 
	make -C src/
	make -C OBJ/

PHONY : clean

clean :
	rm OBJS/*.o OBJS/*.x *.x

ls :
	echo $(EXE)
