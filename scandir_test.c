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

int dig_in(char *path, struct dirent **namelist) {
	char dir_path[256];
	int ndirs;

	memset(dir_path, 0, sizeof dir_path);

	ndirs = scandir(path, &namelist, filter, alphasort);

	if (ndirs < 0) {
		perror("scandir");
		return -1;
	} else {
		while (ndirs--) {
			printf("%s/%s\n", path, namelist[ndirs]->d_name);
			snprintf(dir_path, sizeof dir_path, "%s/%s", path, namelist[ndirs]->d_name);
			dig_in(dir_path, namelist);
			free(namelist[ndirs]);
		}
		free(namelist);
	}

}

int main(int argc, char** argv)
{
	struct dirent **namelist;

	//dig_in("/opt", namelist);
	
	dig_in(argv[1], namelist);

	return 0;
}


