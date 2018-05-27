arfparse is a utility used to parse mailbox archives and extract ARF
information, as described in RFC 5965 "An Extensible Format for Email Feedback
Reports".

It is meant to work as a filter, therefore output of the program is kept as
simple as possible.  Example usage:

arfparse -m ~/mail/aol.net

This will extract ARF information sent from scomp@aol.net assuming the FBL
reports are archived in ~/mail/aol.net

arfparse is developed on OpenBSD with Panda-IMAP and should work with UW-IMAP
too.

- Yiorgos Adamopoulos.
Flames, suggestions and improvements > adamo@ieee.org
