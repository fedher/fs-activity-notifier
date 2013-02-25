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
	mon_params_t *mon_param = (mon_params_t *)arg;
	int wd; 						/* watch descriptor */
	int fd;
	struct inotify_event *event; 	/* fs event */
	char buffer[BUF_LEN];
	int nevents = 0;
	int i;
	notifier_t *notifier = mon_param->notifier;

	if ((fd = inotify_init()) < 0) {
		perror("inotify_init");
		goto err;
	}

	#if DEBUG
	printf("tid: %ld\n", pthread_self());
	printf("path: %s\n", mon_param->dir_path);
	printf("fd: %d\n", fd);
	printf("flags: %d\n", mon_param->flags);
	#endif
	
	memset(buffer, 0, sizeof buffer);

	if ((wd = inotify_add_watch(fd, mon_param->dir_path, mon_param->flags)) < 0) {
		perror("inotify_add_watch");
		goto err;
	}

	while ((nevents = read(fd, buffer, sizeof buffer)) > 0) {
		for (i = 0; i < nevents; i += EVENT_SIZE + event->len) {
			event = (struct inotify_event *) &buffer[i];

			if (event->len) {
				switch (event->mask) {
				case IN_CREATE:
					#if DEBUG
					printf("file created ...");
					#endif
					notifier->oper("file created", notifier->param);
					break;

				case IN_OPEN:
					printf("The file %s was opened.\n", event->name);
					break;

				case IN_DELETE:
					#if DEBUG
					printf("file deleted ...");
					#endif
					notifier->oper("file deleted", notifier->param);
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
