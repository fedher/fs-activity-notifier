#ifndef _MONITOR_H_
#define _MONITOR_H_

#define EVENT_SIZE (sizeof (struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + FILENAME_LEN))

#include "scandir.h"

typedef struct mon_params {
	int flags;
	char dir_path[MAX_DIR_PATH];
} mon_params_t;

/**
 * Monitor to watch the activity over a directory.
 * @arg thread params  
 */
void *monitor(void *arg);

#endif
