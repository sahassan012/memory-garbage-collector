hw4: *.c *.h
	gcc -O0 -g -Wall --std=gnu99 memlib.c mm.c mas.c main.c -o mas
.PHONY:clean
clean:
	rm -f mas
