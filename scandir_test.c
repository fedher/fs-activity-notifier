#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int filter(const struct dirent *e) {
	/* Skips the directorys starting with '.'. */
	if (strncmp(e->d_name, ".", 1) == 0)
		return 0;

	/* Chooses only directories. */
	if (e->d_type == DT_DIR)
		return 1;

	return 0; 
}

int main(void)
{
	struct dirent **namelist;
	int ndirs;

	ndirs = scandir(".", &namelist, filter, alphasort);
	if (ndirs < 0)
		perror("scandir");
	else {
		while (ndirs--) {
			printf("%s\n", namelist[ndirs]->d_name);
			free(namelist[ndirs]);
		}
		free(namelist);
	}
	return 0;
}


