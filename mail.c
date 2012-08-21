/*
 * J. Federico Hernandez <fede.hernandez@gmail.com>
 */
#include <stdio.h>
#include <string.h>

#include "mail.h"

int send_email(const char *msg, notifier_param_t *param)
{
	FILE *email = NULL;	
	char mail_cmd[256];

	memset(mail_cmd, 0, sizeof mail_cmd);

	snprintf(mail_cmd, sizeof mail_cmd, MAIL_CMD, param->to);

	email = popen(mail_cmd, "w");

	fprintf(email, "From: %s\r\n", param->from);
	fprintf(email, "Subject: %s\r\n", param->subject);
	fprintf(email, "%s\r\n", msg);
	fprintf(email, ".\r\n", msg);
	
	pclose(email);
}


