objects = main.o terminal.o highlighting.o editor.o file_io.o output.o input.o lnklist.o

orlando: $(objects) src/data.h
		$(CC) $(objects) -g -o orlando -O0 -Wall -Wextra -pedantic -std=c99

main.o: src/main.c src/defines.h
	$(CC) -c -g src/main.c
terminal.o: src/terminal.c src/defines.h
	$(CC) -c -g src/terminal.c
highlighting.o: src/highlighting.c src/defines.h
	$(CC) -c -g src/highlighting.c
editor.o: src/editor.c src/defines.h
	$(CC) -c -g src/editor.c
file_io.o: src/file_io.c src/defines.h
	$(CC) -c -g src/file_io.c
output.o: src/output.c src/defines.h
	$(CC) -c -g src/output.c
input.o: src/input.c src/defines.h
	$(CC) -c -g src/input.c
lnklist.o: src/lnklist.c
	$(CC) -c -g src/lnklist.c

clean:
	rm $(objects) orlando
