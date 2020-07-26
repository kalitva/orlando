objects = main.o terminal.o syntax.o editor.o file_io.o output.o input.o lnklist.o cursor.o

orlando: $(objects)
		$(CC) $(objects) -g -o orlando -O0 -Wall -Wextra -pedantic -std=c99

main.o: src/main.c src/defines.h
	$(CC) -c -g src/main.c
terminal.o: src/terminal.c src/defines.h
	$(CC) -c -g src/terminal.c
syntax.o: src/syntax.c src/defines.h
	$(CC) -c -g src/syntax.c
editor.o: src/editor.c src/defines.h
	$(CC) -c -g src/editor.c
file_io.o: src/file_io.c src/defines.h
	$(CC) -c -g src/file_io.c
output.o: src/output.c src/defines.h
	$(CC) -c -g src/output.c
input.o: src/input.c src/defines.h
	$(CC) -c -g src/input.c
cursor.o: src/cursor.c src/defines.h
	$(CC) -c -g src/cursor.c
lnklist.o: src/lnklist.c src/defines.h
	$(CC) -c -g src/lnklist.c

clean:
	rm $(objects)
