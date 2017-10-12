#  compiles lager.c
Linux = -gdb 
mac = -lldb 
FLAGS = -Wall -pedantic -g
lager: tree.o list.o lager.c utils.o
	gcc $(FLAGS) $(SYSTEM) lager.c tree.o list.o utils.o -o lager

run:	lager
	./lager

lager_debug:	lager
	$(SYSTEM) ./lager

tree_test: tree.o
	gcc $(SYSTEM) -o tree_test tree_test.c tree.c -lcunit
	./tree_test

list_test: list.o
	gcc $(SYSTEM) -o list_test list_test.c list.c -lcunit
	./list_test

utils.o: utils.c utils.h
	gcc $(SYSTEM) -c utils.c -o utils.o

tree.o: tree.c tree.h common.h
	gcc $(SYSTEM) -c tree.c -o tree.o

list.o: list.c list.h common.h
	gcc $(SYSTEM) -c list.c -o list.o


.PHONY: clean
clean:
	rm -f *.o
	rm -f lager
	rm -f- r *-DSYM
