/*
 * J. Federico Hernandez <fede@rotterdam-cs.com>
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/inotify.h>

#include "monitor.h"
#include "scandir.h"


int main(int argc, char **argv) 
{
	char *cwd = NULL; /* working directory */
	struct dirent **namelist;

	if ((cwd = getcwd(NULL, 0)) == NULL) {
		perror("getcwd()");
		return -2;
	}
	
	#if DEBUG	
	printf("cwd: %s\n", cwd);
	#endif

	/* 
	 * Adds recursively a monitor to each found directory inside of the current
	 * working directory 
	 */
	dir_add_monitors(cwd, namelist, IN_CREATE | IN_DELETE);
	
	if (cwd) free(cwd);
	return 0;
}

