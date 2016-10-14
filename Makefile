all : 
	make -C src/
	make -C OBJ/

PHONY : clean

clean :
	rm OBJ/*.o OBJ/*.x *.x

ls :
	echo $(EXE)
