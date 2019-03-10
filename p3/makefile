#this is a comment
537make: main.o node.o graph.o parse.o execute.o
	gcc -o 537make main.o node.o graph.o parse.o execute.o -g

main.o: main.c main.h node.h parse.h node.o parse.o execute.o
	gcc -c -Wall -Wextra main.c -g

parse.o: parse.c parse.h node.h node.o
	gcc -c -Wall -Wextra parse.c -g

node.o: node.c node.h main.h
	gcc -c -Wall -Wextra node.c -g

#this is a comment
graph.o: graph.c graph.h node.h node.o execute.h execute.o
	gcc -c -Wall -Wextra graph.c -g

execute.o: execute.c execute.h
	gcc -c -Wall -Wextra execute.c -g

clean:
	rm -rf 537make graph.o node.o parse.o main.o execute.o
partialclean:
	rm -rf graph.o node.o parse.o main.o execute.o
