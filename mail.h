#ifndef _MAIL_H_
#define _MAIL_H_

#define MAIL_CMD "/usr/sbin/sendmail -t %s"
#define MAIL_SUBJECT "samba notification"
#define MAIL_TO "fede.hernandez@gmail.com"
#define MAIL_FROM "topo@rotterdam-cs.com"

int send_email(const char *to, const char *subject, const char *msg);

#endif
