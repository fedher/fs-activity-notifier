#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int filter(const struct dirent *e) {
	/* Skips the directorys starting with '.'. */
	if (strncmp(e->d_name, ".", 1) == 0)
		return 0;

	/* Chooses only directories. */
	if (e->d_type == DT_DIR)
		return 1;

	return 0; 
}

int dig_in(char *path, struct dirent **namelist, void *(*monitor)(void *)) {
	char dir_path[256];
	int ndirs;

	memset(dir_path, 0, sizeof dir_path);

	ndirs = scandir(path, &namelist, filter, alphasort);

	if (ndirs < 0) {
		perror("scandir");
		return -1;
	} else {
		while (ndirs--) {
			//printf("%s/%s\n", path, namelist[ndirs]->d_name);
			snprintf(dir_path, sizeof dir_path, "%s/%s", path, namelist[ndirs]->d_name);
			dig_in(dir_path, namelist, monitor);
			free(namelist[ndirs]);
			monitor(dir_path);
		}
		free(namelist);
	}
}

void *mon(void *a) { printf("mon: %s\n", (char *)a); return NULL; }

int main(int argc, char** argv)
{
	struct dirent **namelist;

	//dig_in("/opt", namelist);

	if (argc < 2) {
		printf("Usage: %s <dir>\n", argv[0]);
		return 1;
	}
	
	dig_in(argv[1], namelist, mon);

	return 0;
}


