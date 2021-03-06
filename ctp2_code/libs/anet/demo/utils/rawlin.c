/* 
Copyright (C) 1995-2001 Activision, Inc.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

/*
 *  Copyright (c) 1992 John E. Davis  (davis@amy.tch.harvard.edu)
 *  All Rights Reserved.
 * Munged by Dan Kegel (dank@blacks.jpl.nasa.gov) for use in uwho.
 */
/* mostly copied from rawsun.c; needed by RedHat Linux 5.x */

#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#define TTY_DESCR 0

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* Input buffering done here solely to allow us to handle ^C.
 * This is stupid.
 */
static int KeyBoard_Quit = 0;
int input_buffer_len;
char input_buffer[255];
void mch_settmode(int);

#if 0
static void raw_stuffc(c)
    char c;
{
    input_buffer[input_buffer_len] = c;
    if (input_buffer_len < sizeof(input_buffer))
	input_buffer_len++;
}

static int raw_pullc()
{
   int i;
   char ch;
   if (!input_buffer_len) return(-1);
   ch = input_buffer[0];
   input_buffer_len--;
   for (i = 0; i < input_buffer_len; i++) {
	input_buffer[i] = input_buffer[i + 1];
   }
   return(ch);
}
#endif

/*--------------------------------------------------------------------------
 Call with 1 to install, 0 to deinstall raw mode
--------------------------------------------------------------------------*/
/* set raw_set_stdio to corresponding function from vim */
void raw_set_stdio(init)
    int init;
{
		mch_settmode(init);
}


/* old raw_set_stdio function
void raw_set_stdio(init)
    int init;
{
    struct sgttyb  newtty;
    struct tchars tc;
    struct ltchars lc;
    static struct ltchars  OLDTTY_LC;
    static struct tchars   OLDTTY_TC;
    static struct sgttyb   OLDTTY;

    if (init) {
	ioctl(TTY_DESCR, TIOCGLTC,&OLDTTY_LC);
	ioctl(TTY_DESCR, TIOCGETC,&OLDTTY_TC);
	ioctl(TTY_DESCR, TIOCGETP, &OLDTTY);
	newtty = OLDTTY;
	newtty.sg_flags |= CBREAK;
	newtty.sg_flags &= ~(ECHO);
#if 0
	newtty.sg_erase = 0;
	newtty.sg_kill = 0;
	tc.t_intrc = 0;
	tc.t_quitc = 0;
	tc.t_eofc = 0;
	tc.t_brkc = 0;
	lc.t_suspc = 0;
	lc.t_dsuspc = 0;
#endif
	ioctl(TTY_DESCR, TIOCSETP, &newtty);
	ioctl(TTY_DESCR, TIOCSLTC,&lc);
	ioctl(TTY_DESCR, TIOCSETC,&tc);
    } else {
	ioctl(TTY_DESCR, TIOCSETP, &OLDTTY);
	ioctl(TTY_DESCR, TIOCSLTC, &OLDTTY_LC);
	ioctl(TTY_DESCR, TIOCSETC, &OLDTTY_TC);
    }
}
end of old set_raw_stdio function */

/*--------------------------------------------------------------------------
 Return -1 if no key, 3 if user hit interrupt key, otherwise return key.
--------------------------------------------------------------------------*/
int raw_getc()
{
    char c;

    if (!KeyBoard_Quit && !raw_kbhit())
	return -1;

    if (!KeyBoard_Quit && read(TTY_DESCR, &c, 1) < 0) {
	/* Gack!  Read failed!  Pretend interrupt hit. */
	c = 3;
    }
    if (KeyBoard_Quit) c = 3;
    return(c);
}

/*--------------------------------------------------------------------------
 Return 0 if no key ready, 1 if key ready.
--------------------------------------------------------------------------*/
int raw_kbhit()
{
    struct timeval wait;
    fd_set readfd, writefd, exceptfd;
    int ret;

    /* extern int select(int, int *, int *, int *, struct timeval *); */

    wait.tv_sec = 0;
    wait.tv_usec = 100;	/* should be nonzero to avoid spinning? */

    FD_ZERO(&readfd);
    FD_SET(TTY_DESCR, &readfd);
    FD_ZERO(&writefd);
    FD_ZERO(&exceptfd);

    ret = select(TTY_DESCR + 1, &readfd, &writefd, &exceptfd, &wait);
    return(ret);
}

/*--------------------------------------------------------------------------
 Get the size of the terminal
--------------------------------------------------------------------------*/
void raw_get_term_dimensions(cols, rows)
    int *cols;
    int *rows;
{
    struct winsize wind_struct;
      
   ioctl(2,TIOCGWINSZ,&wind_struct);
   *cols = (int) wind_struct.ws_col;
   *rows = (int) wind_struct.ws_row;
   if (*rows <= 0) *rows = 24;
   if (*cols <= 0) *cols = 80;
}

/* taken from unix.c from VIM v4.5 */

#ifdef TIOCLGET
#undef TIOCLGET
#endif
#ifdef ONLCR
#undef ONLCR
#endif

static int curr_tmode = 1;	/* contains current raw/cooked mode (0 = cooked, 1 = raw) */

	void
mch_settmode(raw)
	int				raw;
{
	static int first = TRUE;

	/* Why is NeXT excluded here (and not in unixunix.h)? */
#if defined(ECHOE) && defined(ICANON) && (defined(_TERMIO_H) || defined(_TERMIOS_H)) && !defined(__NeXT__)
	/* for "new" tty systems */
# ifdef _TERMIOS_H
	static struct termios told;
		   struct termios tnew;
# else
	static struct termio told;
		   struct termio tnew;
# endif

# ifdef TIOCLGET
	static unsigned long tty_local;
# endif

	if (raw)
	{
		if (first)
		{
			first = FALSE;
# ifdef TIOCLGET
			ioctl(0, TIOCLGET, &tty_local);
# endif
# if defined(_TERMIOS_H)
			tcgetattr(0, &told);
# else
			ioctl(0, TCGETA, &told);
# endif
		}
		tnew = told;
		/*
		 * ICRNL enables typing ^V^M
		 */
		tnew.c_iflag &= ~ICRNL;
		tnew.c_lflag &= ~(ICANON | ECHOE | ECHO | IEXTEN);
/*# if defined(IEXTEN) && !defined(MINT)
					| IEXTEN*/		/* IEXTEN enables typing ^V on SOLARIS */
									/* but it breaks function keys on MINT */
/*# endif
								); */
# ifdef ONLCR		/* don't map NL -> CR NL, we do it ourselves */
		tnew.c_oflag &= ~ONLCR;
# endif
		tnew.c_cc[VMIN] = 1;			/* return after 1 char */
		tnew.c_cc[VTIME] = 0;			/* don't wait */
# if defined(_TERMIOS_H)
		tcsetattr(0, TCSANOW, &tnew);
# else
		ioctl(0, TCSETA, &tnew);
# endif
	}
	else
	{
# if defined(_TERMIOS_H)
		tcsetattr(0, TCSANOW, &told);
# else
		ioctl(0, TCSETA, &told);
# endif
# ifdef TIOCLGET
		ioctl(0, TIOCLSET, &tty_local);
# endif
	}
#else
/* removed support for "old" tty systems since requires sgtty.h
   which has overlapping declarations with termios.h*/
#endif
	curr_tmode = raw;
}
