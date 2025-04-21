main: main.c
	gcc -o $@ main.c

memexp: memexp.c
	gcc -O3 -o $@ $<

clean:
	rm -f main
