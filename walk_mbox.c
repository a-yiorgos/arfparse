/* Most of the work is done here.  Read RFC 5965 to understand what is
 * going on.
 *
 * Note #1: I have to check whether to make string comparisons case
 * insensitive. Answer: I do.
 *
 * Note #2: If we are to use mail_fetchbody() then some understanding of
 * section 6.4.5 of RFC3501 is needed.
 *
 * Note #3: From private communication with Mark Crispin we learn that 
 * internals.txt is some 14 years behind. So it is best to determine 
 * structure members and types by looking at c-client.h and mail.h.
 *
 * Note #4: From private communication with Mark Crispin we learn that
 * BODY structures only keep the metatadata for MIME parts and not the
 * actual part itself. For that we need the mail_fetch_???() routines.
 * Maybe it is time that I look over at the alpine source for that and
 * mailpart.c may be the place to start.
 *
 * Note #5: Also worth looking is mtest.c from the Panda-IMAP distribution
 * and especially display_body(). Again from Mark Crispin this.
 *
 * Note #6: After taking a long trip around the alpine-2.00 code, it seems
 * that mail_fetch_body() is my friend. But it comes with a warning:  When
 * you ask for a particular MIME section of the message, you are returned
 * a char * that contains the message from the begining of the said MIME
 * body part till the end of the message. And you have to use the size.bytes
 * information in a BODY structurre that describes the part to tokenize it.
 */

#include <stdio.h>
#include <string.h>
#include "c-client.h"
#include "main.h"

void
walk_mbox(MAILSTREAM *mbox) {
int i;
int len;
ENVELOPE *env;
BODY *body;
PART *part1, *part2, *part3;
PARAMETER *param;
MESSAGECACHE *mc;
char *bodystr;
char *part2str;
char *part3str;
char *pstr;

	part3str = part2str = NULL;

	/* printf("mailbox opened: %d message(s)\n", mbox->nmsgs); /* */

	/* loop through each mail message */
	for (i = 1; i <= mbox->nmsgs; i++) {
		mc = mail_elt(mbox, i);
		env = mail_fetchstructure(mbox, i, &body);

		/* Messages to our interest are of type multipart/report */
		if ((body->type == TYPEMULTIPART) &&
			(strncasecmp("REPORT", body->subtype, 6) == 0)) {

			/* An ARF message consists of three parts:
			 * 1- a human readable one.
			 * 2- a machine readable one.
			 * 3- the abuse message in question
			 *
			 * Read RFC5965 section 2 for more detail.
			 */

			/* The "report-type" parameter of the "multipart/report" type
			 * must be set to "feedback-report"
			 */

			param = body->parameter;
			while (param != NULL) {
				if ((strncasecmp("REPORT-TYPE", param->attribute, 11) == 0) &&
					(strncasecmp("FEEDBACK-REPORT", param->value, 15) == 0)) {
					break;
				}

				param = param->next;
			}
			if (param == NULL) {
				continue; 
			}

			part1 = part2 = part3 = NULL;
			part1 = body->nested.part;

			/* After these two simple ifs, a complicated one follows. When I revisit
			 * This code, I must decide whether I should nest them inside one
			 * another, or complete them with appropriate else sentences that
			 * continue the loop.
			 */

			if (part1) {
				part2 = part1->next;
			}
			if (part2) {
				part3 = part2->next;
			}

			/* WARNING: What follows is a *very* complex if. I welcome 
			 * suggestions that simplyfy it. Otherwise DO NOT TOUCH.
			 */

			if (
				/* Part 1 is a plain message */
				((part1->body.type == TYPETEXT) &&
					(strncasecmp("PLAIN", part1->body.subtype, 5) == 0)) &&

				/* Part 2 is a message/feedback-report */
				((part2->body.type == TYPEMESSAGE) &&
					(strncasecmp("FEEDBACK-REPORT", part2->body.subtype, 15) == 0)) &&

				/* Part 3 is either a "message/rfc822" or a "text/rfc822-headers */
				(
					((part3->body.type == TYPEMESSAGE) &&
						(strncasecmp("RFC822", part3->body.subtype, 6) == 0)) ||
					((part3->body.type == TYPETEXT) &&
						(strncasecmp("RFC822-HEADERS", part3->body.subtype, 14) == 0))
				)
			) {
					/* OK, this is an interesting message, for which we have all
					 * the required metatdata parts in BODY structures.  Now
					 * grab the message body to see how to parse/extract the
					 * actual information needed.
					 */

					bodystr = mail_fetch_body(mbox, i, "2", &len, FT_UID);

					part2str = realloc(part2str, part2->body.size.bytes + 1);
					bzero(part2str, part2->body.size.bytes + 1);
					strncpy(part2str, bodystr, part2->body.size.bytes);

					part3str = realloc(part3str, part3->body.size.bytes + 1);
					bzero(part3str, part3->body.size.bytes + 1);
					strncpy(part3str, bodystr, part3->body.size.bytes);

					printf("X-Idx: %d\n", i);
					printf("X-Subject: %s\n", env->subject);
					pstr = strtok(part2str, "\r\n");
					while (pstr != NULL) {
						if (strncasecmp(pstr, "FEEDBACK-TYPE:", 14) == 0) {
							printf("%s\n", pstr);
						} else if (strncasecmp(pstr, "USER-AGENT:", 11) == 0) {
							printf("%s\n", pstr);
						} else if (strncasecmp(pstr, "VERSION:", 8) == 0) {
							printf("%s\n", pstr);
						} else if (strncasecmp(pstr, "ORIGINAL-ENVELOPE-ID:", 21) == 0) {
							printf("%s\n", pstr);
						} else if (strncasecmp(pstr, "ORIGINAL-MAIL-FROM:", 19) == 0) {
							printf("%s\n", pstr);
						} else if (strncasecmp(pstr, "ARRIVAL-DATE:", 13) == 0) {
							printf("%s\n", pstr);
						} else if (strncasecmp(pstr, "RECEIVED-DATE:", 14) == 0) {
							printf("%s\n", pstr);
						} else if (strncasecmp(pstr, "REPORTING-MTA:", 14) == 0) {
							printf("%s\n", pstr);
						} else if (strncasecmp(pstr, "SOURCE-IP:", 10) == 0) {
							printf("%s\n", pstr);
						} else if (strncasecmp(pstr, "INCIDENTS:", 10) == 0) {
							printf("%s\n", pstr);
						}

						pstr = strtok(NULL, "\r\n");
						if (pstr == NULL) pstr = strtok(NULL, "\r\n");
					}

					/* Now locate Subject: in part3.  In a future date I may
					 * write a decoder for quoted-printable or otherwise
					 * encoded subjects
					 */

					pstr = strtok(part3str, "\r\n");
					while (pstr != NULL) {
						if (strncasecmp(pstr, "Subject:", 8) == 0) {
							printf("%s\n", pstr);
							break;
						}

						pstr = strtok(NULL, "\r\n");
						if (pstr == NULL) pstr = strtok(NULL, "\r\n");
					}

					printf("\n");

				}
		}

		/* Note: use mail_free_body() and mail_free_envelope() here */
	}

	return;
}
