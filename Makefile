OBJS 	= diseaseMonitor.o functions.o LinkedList.o HashTable.o SelfBalancedTree.o Heap.o
SOURCE	= diseaseMonitor.c functions.c LinkedList.c HashTable.c SelfBalancedTree.c Heap.c
HEADER  = definitions.h functions.h LinkedList.h HashTable.h SelfBalancedTree.h Heap.h
OUT  	= diseaseMonitor
CC	= gcc
FLAGS   = -g -c -Wall -std=c99
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

# create/compile the individual files >>separately<< 
diseaseMonitor.o: diseaseMonitor.c
	$(CC) $(FLAGS) diseaseMonitor.c

functions.o: functions.c
	$(CC) $(FLAGS) functions.c

LinkedList.o: LinkedList.c
	$(CC) $(FLAGS) LinkedList.c

HashTable.o: HashTable.c
	$(CC) $(FLAGS) HashTable.c

SelfBalancedTree.o: SelfBalancedTree.c
	$(CC) $(FLAGS) SelfBalancedTree.c

Heap.o: Heap.c
	$(CC) $(FLAGS) Heap.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
