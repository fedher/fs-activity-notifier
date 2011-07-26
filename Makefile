CC = gcc
TARGETS = topo_gigio

all: $(TARGETS)

topo_gigio: inotify.o
		$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


pre:
	sudo apt-get install libesmtp5


clean:
	rm -f $(TARGETS) *.o *~
