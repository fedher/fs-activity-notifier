/*
 * J. Federico Hernandez <fede.hernandez@gmail.com>
 */
#include <stdio.h>
#include <string.h>

#include "mail.h"

int send_email(const char *msg, notifier_t *notifier)
{
	FILE *email = NULL;	
	char mailCmd[256];
	notifier_param_t param = *(notifier->param);

	memset(mailCmd, 0, sizeof mailCmd);

	snprintf(mailCmd, sizeof mailCmd, MAIL_CMD, param.to);

	email = popen(mailCmd, "w");

	fprintf(email, "From: %s\r\n", param.from);
	fprintf(email, "Subject: %s\r\n", param.subject);
	fprintf(email, "%s\r\n", msg);
	fprintf(email, ".\r\n", msg);
	
	pclose(email);
}


