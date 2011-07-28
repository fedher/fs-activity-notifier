#include <dirent.h> 
#include <stdio.h> 

int main(void)
{
	DIR *d;
	struct dirent *dir;
	d = opendir(".");
	
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			printf("%s", dir->d_name);
			if (dir->d_type == DT_DIR) {
				printf(" is a directory.\n");
			} else {
				printf("\n");
			}
		}

		closedir(d);
	}

	return 0;
}

