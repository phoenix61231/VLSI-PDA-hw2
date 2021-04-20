CC = gcc
CFLAGS = -Wall -ansi
SOURCES = main.c file_op.c init.c build_list.c move_cell.c find_min_cut.c check.c
HEADERS = $(SOURCES:.c=.h)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = ../bin/hw2

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $(EXECUTABLE) $(OBJECTS)

main.o : main.c $(HEADERS)
	$(CC) $(CFLAGS) -c main.c

check.o : check.c check.h main.h
	$(CC) $(CFLAGS) -c check.c

file_op.o : file_op.c file_op.h main.h
	$(CC) $(CFLAGS) -c file_op.c

init.o : init.c init.h main.h
	$(CC) $(CFLAGS) -c init.c

build_list.o : build_list.c build_list.h main.h
	$(CC) $(CFLAGS) -c build_list.c

move_cell.o : move_cell.c move_cell.h build_list.h main.h
	$(CC) $(CFLAGS) -c move_cell.c

find_min_cut.o : find_min_cut.c find_min_cut.h move_cell.h main.h
	$(CC) $(CFLAGS) -c find_min_cut.c

clean:
	rm $(EXECUTABLE) $(OBJECTS) 
