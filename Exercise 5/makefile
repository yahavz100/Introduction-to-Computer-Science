a.out: main.o stack.o queue.o strings.o element.o
	gcc -std=c99 main.o element.o stack.o queue.o strings.o
main.o: main.c
	gcc -c -std=c99 main.c
element.o: element.c element.h
	gcc -c -std=c99 element.c
stack.o: stack.c stack.h
	gcc -c -std=c99 stack.c
queue.o: queue.c queue.h
	gcc -c -std=c99 queue.c
strings.o: strings.c strings.h
	gcc -c -std=c99 strings.c
clean:
	rm -f *.o a.out
run: a.out
	./a.out
