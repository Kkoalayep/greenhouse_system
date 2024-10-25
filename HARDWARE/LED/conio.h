#ifndef __CONIO_H
#define __CONIO_H 			   
#include "stm32f4xx.h"

　　cgets(char *);
　　cprintf(const char *, ...);
　　cputs(const char *);
　　cscanf(const char *, ...);
　　inp(unsigned short);
　　inpw(unsigned short);
　　getch(void);
　　getche(void);
　　kbhit(void);
　　outp(unsigned short, int);
　　outpw(unsigned short, unsigned short);
　　putch(int);
　　ungetch(int);
　　void _Cdecl clreol (void);
　　void _Cdecl clrscr (void);
　　void _Cdecl delline (void);
　　int _Cdecl gettext (int left, int top, int right, int bottom,
　　void *destin);
　　void _Cdecl gettextinfo (struct text_info *r);
　　void _Cdecl gotoxy (int x, int y);
　　void _Cdecl highvideo (void);
　　void _Cdecl insline (void);
　　void _Cdecl lowvideo (void);
　　int _Cdecl movetext (int left, int top, int right, int bottom,
　　int destleft, int desttop);
　　void _Cdecl normvideo (void);
　　int _Cdecl puttext (int left, int top, int right, int bottom,
　　void *source);
　　void _Cdecl textattr (int newattr);
　　void _Cdecl textbackground (int newcolor);
　　void _Cdecl textcolor (int newcolor);
　　void _Cdecl textmode (int newmode);
　　int _Cdecl wherex (void);
　　int _Cdecl wherey (void);
　　void _Cdecl window (int left, int top, int right, int bottom);
　　har *_Cdecl cgets (char *str);
　　int _Cdecl cprintf (const char *format, ...);
　　int _Cdecl cputs (const char *str);
　　int _Cdecl cscanf (const char *format, ...);
　　int _Cdecl getch (void);
　　int _Cdecl getche (void);
　　char *_Cdecl getpass (const char *prompt);
　　int _Cdecl kbhit (void);
　　int _Cdecl putch (int c);
　　int _Cdecl ungetch (int ch);
#endif