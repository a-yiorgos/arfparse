#include <stdio.h>
#include "c-client.h"

/* This is taken from imap-2007e mtest.c */

/* MM display body
 * Accepts: BODY structure pointer
 *	    prefix string
 *	    index
 */

void display_body (BODY *body, char *pfx, long i)
{
  char tmp[MAILTMPLEN];
  char *s = tmp;
  PARAMETER *par;
  PART *part;			/* multipart doesn't have a row to itself */
  if (body->type == TYPEMULTIPART) {
				/* if not first time, extend prefix */
    if (pfx) sprintf (tmp,"%s%ld.",pfx,++i);
    else tmp[0] = '\0';
    for (i = 0,part = body->nested.part; part; part = part->next)
      display_body (&part->body,tmp,i++);
  }
  else {			/* non-multipart, output oneline descriptor */
    if (!pfx) pfx = "";		/* dummy prefix if top level */
    sprintf (s," %s%ld %s",pfx,++i,body_types[body->type]);
    if (body->subtype) sprintf (s += strlen (s),"/%s",body->subtype);
    if (body->description) sprintf (s += strlen (s)," (%s)",body->description);
    if (par = body->parameter) do
      sprintf (s += strlen (s),";%s=%s",par->attribute,par->value);
    while (par = par->next);
    if (body->id) sprintf (s += strlen (s),", id = %s",body->id);
    switch (body->type) {	/* bytes or lines depending upon body type */
    case TYPEMESSAGE:		/* encapsulated message */
    case TYPETEXT:		/* plain text */
      sprintf (s += strlen (s)," (%lu lines)",body->size.lines);
      break;
    default:
      sprintf (s += strlen (s)," (%lu bytes)",body->size.bytes);
      break;
    }
    puts (tmp);			/* output this line */
				/* encapsulated message? */
    if ((body->type == TYPEMESSAGE) && !strcmp (body->subtype,"RFC822") &&
	(body = body->nested.msg->body)) {
      if (body->type == TYPEMULTIPART) display_body (body,pfx,i-1);
      else {			/* build encapsulation prefix */
	sprintf (tmp,"%s%ld.",pfx,i);
	display_body (body,tmp,(long) 0);
      }
    }
  }
}
