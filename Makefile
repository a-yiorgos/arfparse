# Using Panda IMAP to link
CFLAGS		= -g -I../../imap/panda/imap-2010/c-client
LDFLAGS		= ../../imap/panda/imap-2010/c-client/c-client.a -lssl -lcrypto

# Debian systems and UW-IMAP (via Sotiris Tsimbonis)
# apt-get install libc-client2007e libc-client2007e-dev
# CFLAGS	= -g -I/usr/include/c-client
# LDFLAGS	= -lc-client -lssl -lcrypto

OBJS		= main.o mm_callbacks.o walk_mbox.o

all: arfparse

arfparse: $(OBJS)
	$(CC) $(CFLAGS) -o arfparse $(OBJS) $(LDFLAGS)

clean:
	rm -rf arfparse *.o core *.core
