/*
 * J. Federico Hernandez <fede@rotterdam-cs.com>
 */
#include <unistd.h>
#include <sys/inotify.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <dirent.h>


#define FILENAME_LEN 80
#define EVENT_SIZE (sizeof (struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + FILENAME_LEN))


#define MAIL_CMD "/usr/sbin/sendmail -t %s"
#define MAIL_SUBJECT "samba notification"
#define MAIL_TO "fede.hernandez@gmail.com"
#define MAIL_FROM "topo@rotterdam-cs.com"


typedef struct mon_params {
	int fd;
	int flags;
	char dir_path[256];
} mon_params_t;


void *monitor(void *arg);
int send_email(const char *to, const char *subject, const char *msg);
int dir_filter(const struct dirent *e);
int dir_add_monitors(char *path, struct dirent **namelist, int fd, int flags);


int main(int argc, char **argv) 
{
	int fd;
	char *cwd = NULL; // working directory
	struct dirent **namelist;

	if ((fd = inotify_init()) < 0) {
		perror("inotify_init");
		return -1;
	}

	if ((cwd = getcwd(NULL, 0)) == NULL) {
		perror("getcwd()");
		return -2;
	}
	
	printf("cwd: %s\n", cwd);

	/* Sets a monitor by each directory */
	dir_add_monitors(cwd, namelist, fd, IN_CREATE | IN_DELETE);
	
	close(fd);
	if (cwd) free(cwd);

	return 0;
}


void *monitor(void *arg) 
{
	mon_params_t param = *(mon_params_t *)arg;
	int wd; // watch descriptor
	struct inotify_event *event;
	char buffer[BUF_LEN];
	int nevents = 0;
	int i;

	//pthread_detach();
	memset(buffer, 0, sizeof buffer);

	if ((wd = inotify_add_watch(param.fd, param.dir_path, param.flags)) < 0) {
		perror("inotify_add_watch");
		pthread_exit(NULL);
	}

	while ((nevents = read(param.fd, buffer, sizeof buffer)) > 0) {
		for (i = 0; i < nevents; i += EVENT_SIZE + event->len) {
			event = (struct inotify_event *) &buffer[i];

			if (event->len) {
				switch (event->mask) {
					case IN_CREATE:
						//send_email(MAIL_TO, MAIL_SUBJECT, "file created.");
						printf("The file %s was created.\n", event->name);
						break;

					case IN_OPEN:
						printf("The file %s was opened.\n", event->name);
						break;

					case IN_DELETE:
						//send_email(MAIL_TO, MAIL_SUBJECT, "file deleted.");
						printf("The file %s was deleted.\n", event->name);
						break;
				}
			}
		}
	}

	inotify_rm_watch(param.fd, wd);
	if (arg) free(arg);
	pthread_exit(NULL);
}


int send_email(const char *to, const char *subject, const char *msg)
{
	FILE *email = NULL;	
	char mailCmd[256];

	memset(mailCmd, 0, sizeof mailCmd);

	snprintf(mailCmd, sizeof mailCmd, MAIL_CMD, to);

	email = popen(mailCmd, "w");

	fprintf(email, "From: %s\r\n", MAIL_FROM);
	fprintf(email, "Subject: %s\r\n", subject);
	fprintf(email, "%s\r\n", msg);
	fprintf(email, ".\r\n", msg);
	
	pclose(email);
}

int filter_dir(const struct dirent *e) {
	/* Skips the directorys starting with '.'. */
	if (strncmp(e->d_name, ".", 1) == 0)
		return 0;

	/* Chooses only directories. */
	if (e->d_type == DT_DIR)
		return 1;

	return 0; 
}

int dir_add_monitors(char *path, struct dirent **namelist, int fd, int flags) {
	char dir_path[256];
	int ndirs;
	mon_params_t *m_param;
	pthread_t tid;	

	memset(dir_path, 0, sizeof dir_path);

	ndirs = scandir(path, &namelist, filter_dir, alphasort);

	if (ndirs < 0) {
		perror("scandir");
		return -1;
	} else {
		while (ndirs--) {
			//printf("%s/%s\n", path, namelist[ndirs]->d_name);
			snprintf(dir_path, sizeof dir_path, "%s/%s", path, namelist[ndirs]->d_name);
			dir_add_monitors(dir_path, namelist, fd, flags);
			free(namelist[ndirs]);
		
			if ((m_param = malloc(sizeof m_param)) == NULL) {
				perror("malloc()");
				return -2;	
			}
		
			m_param->fd = fd;
			m_param->flags = flags;
			strncpy(m_param->dir_path, dir_path, sizeof m_param->dir_path);
	
			pthread_create(&tid, NULL, monitor, (void *)m_param);
			//monitor(dir_path);
		}
		free(namelist);
	}
}
