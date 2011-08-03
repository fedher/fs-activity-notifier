CC = gcc
TARGETS = topogigio scandir_test readdir_test

all: $(TARGETS)

topogigio: inotify.o
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
