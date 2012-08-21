#
# J. Federico Hernandez <fede.hernandez@gmail.com>
#

CC = gcc
TARGETS = topogigio
LDLIBS = -lpthread
CFLAGS = -DDEBUG=1

all: $(TARGETS)

topogigio: inotify.o scandir.o monitor.o mail.o
		$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


pre:
	sudo apt-get install libesmtp6


test: topogigio
	./topogigio


clean:
	rm -f $(TARGETS) *.o *~
