SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
EXEC=detect


%.o : %.c 
	gcc -c $^ -o $@ -g

$(EXEC) : $(OBJECTS)
	gcc $(OBJECTS) -o $@ -Wall -g
	
clean :
	rm $(OBJECTS) $(EXEC)