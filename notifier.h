#ifndef _NOTIFIER_H
#define _NOTIFIER_H

typedef struct {
	char *from;
	char *to;
	char *subject;
} notifier_param_t;


typedef struct {
	int (*oper)(const char *msg, notifier_param_t *p);
	notifier_param_t *param;
} notifier_t;

#endif
