# format of makefile:
# target: dependencies
#	command when dependencies have changed

all: trace

trace: trace.c LC4.o ObjectFiles.o
	clang -o trace trace.c LC4.o ObjectFiles.o

LC4.o: LC4.h LC4.c
	clang -c LC4.c

ObjectFiles.o: ObjectFiles.h ObjectFiles.c
	clang -c ObjectFiles.c

clean:
	rm -rf *.o

clobber: clean
	rm -rf trace