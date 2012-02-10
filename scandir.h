#ifndef _SCANDIR_H_
#define _SCANDIR_H_

#define MAX_DIR_PATH 256
#define FILENAME_LEN 80
#define MAX_DIR_NUMBER 64

#include <dirent.h>

#include "notifier.h"

/**
 * Creates a monitor for each found directory.
 * @path directory path
 * @namelist
 * @flags
 * @notifier
 */
int dir_add_monitors(char *path, struct dirent **namelist, int flags, 
				notifier_t *notifier);

#endif
