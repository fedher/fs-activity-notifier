/*
 * J. Federico Hernandez <fede@rotterdam-cs.com>
 */
#include <unistd.h>
#include <sys/inotify.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>


#define FILENAME_LEN 24
#define EVENT_SIZE (sizeof (struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + FILENAME_LEN))

#define MAIL_CMD "/usr/bin/mail"
#define MAIL_SUBJECT "samba notification"
#define MAIL_TO "fede.hernandez@gmail.com"


typedef struct mon_params {
	int fd;
	int flags;
} mon_params_t;


void *monitor(void *arg);
int send_email(const char *to, const char *subject, const char *msg);

int main(int argc, char **argv) 
{
	int fd;
	int wd; // watch descriptor
	struct inotify_event *event;
	char buffer[BUF_LEN];
	int nevents = 0;
	int i;

	memset(buffer, 0, sizeof buffer);

	if ((fd = inotify_init()) < 0) {
		perror("inotify_init");
		return -1;
	}

	/* Add a watcher */
	wd = inotify_add_watch(fd, "./", IN_CREATE | IN_DELETE);

	if (wd < 0) {
		perror("inotify_add_watch");
		return -2;
	}

	while ((nevents = read(fd, buffer, sizeof buffer)) > 0) {
		for (i = 0; i < nevents; i += EVENT_SIZE + event->len) {
			event = (struct inotify_event *) &buffer[i];

			if (event->len) {
				switch (event->mask) {
					case IN_CREATE:
						printf("The file %s was created.\n", event->name);
						break;

					case IN_OPEN:
						printf("The file %s was opened.\n", event->name);
						break;

					case IN_DELETE:
						printf("The file %s was deleted.\n", event->name);
						break;
				}
			}
		}
	}
	
	inotify_rm_watch(fd, wd);
	close(fd);

	return 0;
}


void *monitor(void *arg) 
{
	mon_params_t params = *(mon_params_t *)arg;
	int wd;
		
	// @@ TODO: implement this!!

	pthread_exit(NULL);
}


int send_email(const char *to, const char *subject, const char *msg)
{
	char mailCmd[256];
	FILE *email = NULL;	

	memset(mailCmd, 0, sizeof mailCmd);

	snprintf(mailCmd, sizeof mailCmd, "%s -s '%s' %s", MAIL_CMD, MAIL_SUBJECT, MAIL_TO);
	email = popen(mailCmd, "w");
	fprintf(email, "%s\n", msg);
	
	pclose(email);
}
