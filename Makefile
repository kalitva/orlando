objects = main.o terminal.o highlighting.o editor.o file_io.o output.o input.o lnklist.o

orlando: $(objects) src/data.h
		$(CC) $(objects) -g -o orlando -Wall -Wextra -pedantic -std=c99

main.o: src/main.c src/defines.h
	$(CC) -c src/main.c
terminal.o: src/terminal.c src/defines.h
	$(CC) -c src/terminal.c
highlighting.o: src/highlighting.c src/defines.h
	$(CC) -c src/highlighting.c
editor.o: src/editor.c src/defines.h
	$(CC) -c src/editor.c
file_io.o: src/file_io.c src/defines.h
	$(CC) -c src/file_io.c
output.o: src/output.c src/defines.h
	$(CC) -c src/output.c
input.o: src/input.c src/defines.h
	$(CC) -c src/input.c
lnklist.o: src/lnklist.c
	$(CC) -c src/lnklist.c

clean:
	rm $(objects) orlando
