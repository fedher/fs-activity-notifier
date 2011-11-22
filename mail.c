/*
 * J. Federico Hernandez <fede@rotterdam-cs.com>
 */
#include <stdio.h>
#include <string.h>

#include "mail.h"

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


