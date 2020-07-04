objects = orlando.o terminal.o highlighting.o row_operations.o editor_operations.o file_io.o output.o input.o

orlando: $(objects) data.h
		$(CC) $(objects) -g -o orlando -Wall -Wextra -pedantic -std=c99

orlando.o: orlando.c defines.h
	$(CC) -c orlando.c -include defines.h
terminal.o: terminal.c defines.h
	$(CC) -c terminal.c -include defines.h
highlighting.o: highlighting.c defines.h
	$(CC) -c highlighting.c -include defines.h
row_operations.o: row_operations.c defines.h
	$(CC) -c row_operations.c -include defines.h 
editor_operations.o: editor_operations.c defines.h
	$(CC) -c editor_operations.c -include defines.h
file_io.o: file_io.c defines.h
	$(CC) -c file_io.c -include defines.h
output.o: output.c defines.h
	$(CC) -c output.c -include defines.h
input.o: input.c defines.h
	$(CC) -c input.c -include defines.h

clean:
	rm $(objects) orlando
