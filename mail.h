#ifndef _MAIL_H_
#define _MAIL_H_

#define MAIL_CMD "/usr/sbin/sendmail -t %s"
#define MAIL_SUBJECT "samba notification"
#define MAIL_TO "fede.hernandez@gmail.com"
#define MAIL_FROM "topo@rotterdam-cs.com"

#include "notifier.h"

int send_email(const char *msg, notifier_t *n);

#endif
