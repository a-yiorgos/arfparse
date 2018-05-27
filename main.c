#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "c-client.h"
#include "main.h"

char _prog[] = "arfparse";

void
usage() {
	fprintf(stderr, "usage: %s [options]\n", _prog);
	fprintf(stderr, "\t-m mailbox\n");
}

int
main(int argc, char **argv) {
int ch;
int m_flag;
MAILSTREAM *mbox;
char *infile;

#include "linkage.c"

	m_flag = 0;
	while ((ch = getopt(argc, argv, "m:")) != -1) {
		switch(ch) {
		case 'm':
			m_flag = 1;
			infile = optarg;
			break;
		default:
			usage();
			exit(1);
		}
	}
	argc -= optind;
	argv += optind;

	if (!m_flag) {
		fprintf(stderr, "You must specify an input mailbox\n");
		usage();
		exit(1);
	}

	mbox = mail_open(NIL, infile, OP_READONLY);
	if (mbox == NIL) {
		fprintf(stderr, "mail_open(%s) failed.\n", infile);
		exit(1);
	}

	walk_mbox(mbox);

	mail_close(mbox);
	exit(0);
}
