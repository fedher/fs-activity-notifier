#
# J. Federico Hernandez <fede@rotterdam-cs.com>
#

CC = gcc
TARGETS = topogigio scandir_test readdir_test
LDLIBS = -lpthread
CFLAGS = -DDEBUG=1

all: $(TARGETS)

topogigio: inotify.o scandir.o monitor.o mail.o
		$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


scandir_test: scandir_test.c
		$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


readdir_test: readdir_test.c
		$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


pre:
	sudo apt-get install libesmtp5


test: topogigio
	./topogigio


test-scan: scandir_test
	./scandir_test /opt

test-read: readdir_test
	./readdir_test

clean:
	rm -f $(TARGETS) *.o *~
