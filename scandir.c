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

int dir_add_monitors(char *path, struct dirent **namelist, int flags, 
				notifier_t *notifier) 
{
	char dir_path[MAX_DIR_PATH];
	int ndirs; 				/* directories number */
	int nths;				/* threads number */
	mon_params_t *m_param; 	/* monitor params */
	pthread_t tid[MAX_DIR_NUMBER]; 
	int i;

	memset(dir_path, 0, sizeof dir_path);
	memset(tid, 0, sizeof tid);

	/* Looks for directories */
	nths = ndirs = scandir(path, &namelist, filter_dir, alphasort);

	if (ndirs < 0) {
		perror("scandir");
		return -1;
	} 

	/* Creates a monitor for each directory */
	while (ndirs--) {
		#if DEBUG
		printf("%s/%s\n", path, namelist[ndirs]->d_name);
		#endif

		snprintf(dir_path, sizeof dir_path, "%s/%s", path, namelist[ndirs]->d_name);
		dir_add_monitors(dir_path, namelist, flags, notifier);
		free(namelist[ndirs]);

		if ((m_param = malloc(sizeof *m_param)) == NULL) {
			perror("malloc()");
			return -3;	
		}

		/* monitor params */
		m_param->flags = flags;
		strncpy(m_param->dir_path, dir_path, sizeof m_param->dir_path);
		m_param->notifier = notifier;

		/* monitor creation */	
		pthread_create(&tid[ndirs], NULL, monitor, (void *)m_param);
	}
	free(namelist);

	for (i = 0; i < nths; i++)
		pthread_join(tid[i], NULL);

	return 0;
}
