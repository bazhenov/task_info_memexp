main: main.c
	gcc -o $@ main.c

clean:
	rm -f main
