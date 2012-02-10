/*
 * J. Federico Hernandez <fede.hernandez@gmail.com>
 */
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/inotify.h>
#include <string.h>
#include <stdlib.h>

#include "monitor.h"
#include "notifier.h"


void *monitor(void *arg) 
{
	mon_params_t param = *(mon_params_t *)arg;
	int wd; /* watch descriptor */
	int fd;
	struct inotify_event *event; /* fs event */
	char buffer[BUF_LEN];
	int nevents = 0;
	int i;
	notifier_t *notifier = param.notifier;

	if ((fd = inotify_init()) < 0) {
		perror("inotify_init");
		goto err;
	}

	#if DEBUG
	printf("tid: %ld\n", pthread_self());
	printf("dir: %s\n", param.dir_path);
	printf("dir: %d\n", fd);
	printf("dir: %d\n", param.flags);
	#endif

	memset(buffer, 0, sizeof buffer);

	if ((wd = inotify_add_watch(fd, param.dir_path, param.flags)) < 0) {
		perror("inotify_add_watch");
		goto err;
	}

	while ((nevents = read(fd, buffer, sizeof buffer)) > 0) {
		for (i = 0; i < nevents; i += EVENT_SIZE + event->len) {
			event = (struct inotify_event *) &buffer[i];

			if (event->len) {
				switch (event->mask) {
				case IN_CREATE:
					//send_email(MAIL_TO, MAIL_SUBJECT, "file created.");
					notifier->oper("file created", notifier->param);
					printf("The file %s was created.\n", event->name);
					break;

				case IN_OPEN:
					printf("The file %s was opened.\n", event->name);
					break;

				case IN_DELETE:
					//send_email(MAIL_TO, MAIL_SUBJECT, "file deleted.");
					notifier->oper("file deleted", notifier->param);
					printf("The file %s was deleted.\n", event->name);
					break;
				}
			}
		}
	}
	
	inotify_rm_watch(fd, wd);

err:
	if (arg) free(arg);
	close(fd);
	pthread_exit(NULL);
}
