objects = main.o terminal.o editor.o  cursor.o file_io.o output.o input.o lnklist.o stack.o syntax.o 

orlando: $(objects)
	$(CC) $(objects) -g -o orlando -O0 -Wall -Wextra -pedantic -lncurses -std=c99

main.o: main.c global.h
	$(CC) -c -g main.c

terminal.o: core/terminal.c global.h
	$(CC) -c -g core/terminal.c -include global.h
editor.o: core/editor.c global.h
	$(CC) -c -g core/editor.c -include global.h
cursor.o: core/cursor.c global.h
	$(CC) -c -g core/cursor.c -include global.h

file_io.o: io/file_io.c global.h
	$(CC) -c -g io/file_io.c -include global.h
output.o: io/output.c global.h
	$(CC) -c -g io/output.c -include global.h
input.o: io/input.c global.h
	$(CC) -c -g io/input.c -include global.h

lnklist.o: dtypes/lnklist.c dtypes/types.h
	$(CC) -c -g dtypes/lnklist.c
stack.o: dtypes/stack.c dtypes/types.h
	$(CC) -c -g dtypes/stack.c

syntax.o: syntax.c global.h
	$(CC) -c -g syntax.c -include global.h

clean:
	rm $(objects)