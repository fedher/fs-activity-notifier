#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <dirent.h>

#include "scandir.h"
#include "monitor.h"

/**
 * Returns true when the file is a directory.
 * @e dirent
 * @return
 */
static int filter_dir(const struct dirent *e) 
{
	/* Skips the directorys starting with '.' */
	if (strncmp(e->d_name, ".", 1) == 0)
		return 0;

	/* Chooses only directories. */
	if (e->d_type == DT_DIR)
		return 1;

	return 0; 
}

static void create_monitor(pthread_t *tid, char *dir_path, int flags, 
	notifier_t *notifier)
{
	mon_params_t *m_param; 	/* monitor params */

	if ((m_param = malloc(sizeof *m_param)) == NULL) {
		perror("malloc()");
		return;
	}

	/* monitor params */
	m_param->flags = flags;
	strncpy(m_param->dir_path, dir_path, sizeof m_param->dir_path);
	m_param->notifier = notifier;

	/* monitor creation */	
	pthread_create(tid, NULL, monitor, (void *)m_param);
}

int dir_add_monitors(char *path, struct dirent **namelist, int flags, 
				notifier_t *notifier) 
{
	static int rec_depth = 0;		/* recursion depth */
	static int rec_count = 0;		/* recursion count */
	char dir_path[MAX_DIR_PATH];
	char *dir_name;
	int ndirs;						/* directories number */
	static int tidx = 0;			/* thread id */
	static pthread_t tid[MAX_DIR_NUMBER]; 
	int i;

	if (rec_depth >= RECUR_DEPTH) {
		--rec_depth;
		return 0;
	}

	++rec_depth;

	memset(dir_path, 0, sizeof dir_path);
	memset(tid, 0, sizeof tid);

	/* Looks for directories */
	ndirs = scandir(path, &namelist, filter_dir, alphasort);

	if (ndirs < 0) {
		perror("scandir");
		return -1;
	}

	#if DEBUG
	printf("ndirs: %d\n", ndirs);
	printf("tidx: %d\n", tidx);
	#endif

	/* Adds a monitor to the current directory and returns */
	if (ndirs == 0) {
		create_monitor(&tid[tidx++], path, flags, notifier);
		return 0;
	}

	rec_count++;

	/* Creates a monitor for each directory */
	while (ndirs--) {
		dir_name = namelist[ndirs]->d_name;

		#if DEBUG
		printf("(%d) %s/%s\n", ndirs, path, dir_name);
		#endif

		snprintf(dir_path, sizeof dir_path, "%s/%s", path, dir_name);

		dir_add_monitors(dir_path, namelist, flags, notifier);

		if (namelist[ndirs]) free(namelist[ndirs]);
		
		/* Creates a thread to monitor the directory 'dir_path' */	
		create_monitor(&tid[tidx++], dir_path, flags, notifier);
	}

	rec_count--;

	if (namelist) free(namelist);

	/* 
	 * When the recursion ends, the first function call will wait for the 
	 * launched threads.
	 */
	if (rec_count == 0) {
		while (tidx--) {
			pthread_join(tid[tidx], NULL);
		}
	}

	return 0;
}
