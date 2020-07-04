objects = orlando.o terminal.o highlighting.o row_operations.o editor_operations.o file_io.o output.o input.o

orlando: $(objects) src/data.h
		$(CC) $(objects) -g -o orlando -Wall -Wextra -pedantic -std=c99

orlando.o: src/orlando.c src/defines.h
	$(CC) -c src/orlando.c -include src/defines.h
terminal.o: src/terminal.c src/defines.h
	$(CC) -c src/terminal.c -include src/defines.h
highlighting.o: src/highlighting.c src/defines.h
	$(CC) -c src/highlighting.c -include src/defines.h
row_operations.o: src/row_operations.c src/defines.h
	$(CC) -c src/row_operations.c -include src/defines.h 
editor_operations.o: src/editor_operations.c src/defines.h
	$(CC) -c src/editor_operations.c -include src/defines.h
file_io.o: src/file_io.c src/defines.h
	$(CC) -c src/file_io.c -include src/defines.h
output.o: src/output.c src/defines.h
	$(CC) -c src/output.c -include src/defines.h
input.o: src/input.c src/defines.h
	$(CC) -c src/input.c -include src/defines.h

clean:
	rm $(objects) orlando
