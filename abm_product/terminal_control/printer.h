/*VER: 1*/ 
/* Copyright (c) 2001-<2003> Wholewise, All rights Reserved */
/* #ident	"@(#)printer.h	1.0	2001/02/01 ruanyongjiang" */

#ifndef    _PRINTER_H
#define    _PRINTER_H

#define    OPENLP         { system("stty ixon ixany");\
                             printf("\033[5i"); \
                           }
#define    CLOSELP         { printf("\033[4i");  }
#define    SETLINE(n)      printf("\033J%c",n)   /*�趨�м��*/
#define    OUTPAPER        printf("");         /* ��ֽ */
#define    HIGHFONT        printf("\033c")       /* ����*/
#define    UHIGHFONT       printf("\033d")       /* �� ��*/
#define    BIGFONT         printf("\033h")       /* ����*/
#define    UBIGFONT        printf("\033d")       /* �� ��*/

#endif

