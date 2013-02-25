/*
 * J. Federico Hernandez <fede.hernandez@gmail.com>
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/inotify.h>

#include "monitor.h"
#include "scandir.h"
#include "notifier.h"
#include "mail.h"

int _send_email(const char *msg, notifier_param_t *param) 
{
	send_email(msg, param);
	printf("email: %s\n", msg);
	return 0;
}


int main(int argc, char **argv) 
{
	char *working_dir = NULL; /* working directory */
	struct dirent **namelist;

	notifier_param_t notif_param = {
		.from = MAIL_FROM,
		.to = MAIL_TO,
		.subject = MAIL_SUBJECT
	};

	notifier_t notifier = {
		.oper = _send_email,
		.param = &notif_param 
	};

	working_dir = argv[1] ? argv[1] : getcwd(NULL, 0);

	if (working_dir == NULL) {
		perror("getcwd()");
		return -2;
	}
	
	#if DEBUG	
	printf("working_dir: %s\n", working_dir);
	#endif

	/* 
	 * Adds recursively a monitor to each found directory inside of the current
	 * working directory 
	 */
	dir_add_monitors(working_dir, namelist, IN_CREATE | IN_DELETE, &notifier);
	
	if (working_dir) free(working_dir);
	return 0;
}

