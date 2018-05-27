#include "c-client.h"

void
mm_flags(MAILSTREAM *stream,unsigned long number) {
}

void
mm_status(MAILSTREAM *stream,char *mailbox,MAILSTATUS *status) {
}

void
mm_searched(MAILSTREAM *stream,unsigned long number) {
}

void
mm_exists(MAILSTREAM *stream,unsigned long number) {
}

void
mm_expunged(MAILSTREAM *stream,unsigned long number) {
}

void
mm_list(MAILSTREAM *stream,int delimiter,char *name,long attributes) {
}

void
mm_lsub(MAILSTREAM *stream,int delimiter,char *name,long attributes) {
}

void
mm_notify(MAILSTREAM *stream,char *string,long errflg) {
}

void
mm_log(char *string,long errflg) {
}

void
mm_dlog(char *string) {
}

void
mm_login(NETMBX *mb,char *user,char *pwd,long trial) {
}

void
mm_critical(MAILSTREAM *stream) {
}

void
mm_nocritical(MAILSTREAM *stream) {
}

long
mm_diskerror(MAILSTREAM *stream,long errcode,long serious) {
}

void
mm_fatal(char *string) {
}
