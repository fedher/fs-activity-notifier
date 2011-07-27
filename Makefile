CC = gcc
TARGETS = topogigio

all: $(TARGETS)

topogigio: inotify.o
		$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


pre:
	sudo apt-get install libesmtp5


test: topogigio
	./topogigio


clean:
	rm -f $(TARGETS) *.o *~
