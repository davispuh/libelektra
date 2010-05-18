/***************************************************************************
                   sig.h  - Signal processing
                             -------------------
    copyright            : (C) 2006 by Yannick Lecaillez
    email                : sizon5@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the BSD License (revised).                      *
 *                                                                         *
 ***************************************************************************/


/* Subversion stuff

$Id$

*/

extern int sig_alarm;
extern int sig_child;
extern int sig_cont;
extern int sig_hangup;
extern int sig_int;
extern int sig_pipe;
extern int sig_term;

void sig_block(int sig);
void sig_unblock(int sig);
void sig_blocknone(void);
void sig_catch(int sig,void (*f)());
void sig_pause(void);
extern void (*sig_defaulthandler)();
extern void (*sig_ignorehandler)();

#define sig_ignore(s) (sig_catch((s),sig_ignorehandler))
#define sig_uncatch(s) (sig_catch((s),sig_defaulthandler))