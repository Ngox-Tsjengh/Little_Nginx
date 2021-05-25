#include <stdio.h>
#include <string.h>

#include "ngx_macro.h"
#include "ngx_string.h"

static unsigned long long getuint(va_list *ap, int lflag);
static long long getint(va_list *ap, int lflag);


//Intercepts the tail space of a string
void Rtrim(char *string)   
{   
	size_t len = 0;   
	if(string == NULL)   
		return;   

	len = strlen(string);   
	while(len > 0 && string[len-1] == ' ')
		string[--len] = 0;   
	return;   
}

//Intercepts the head space of a string
void Ltrim(char *string)
{
	size_t len = 0;
	len = strlen(string);   
	char *p_tmp = string;
	if( (*p_tmp) != ' ') 	return; //Not begin with space

	//Find first one that begin with space
	while((*p_tmp) != '\0')
	{
		if( (*p_tmp) == ' ') p_tmp++;
		else				 break;
	}
	if((*p_tmp) == '\0') //full of space
	{
		*string = '\0';
		return;
	}
	char *p_tmp2 = string; 
	while((*p_tmp) != '\0')
	{
		(*p_tmp2) = (*p_tmp);
		p_tmp++;
		p_tmp2++;
	}
	(*p_tmp2) = '\0';
    return;
}

/* *
 * printnum - print a number (base <= 16) in reverse order
 * @putch:        specified putch function, print a single character
 * @putdat:        used by @putch function
 * @num:        the number will be printed
 * @base:        base for print, must be in [1, 16]
 * @width:         maximum number of digits, if the actual width is less than @width, use @padc instead
 * @padc:        character that padded on the left if the actual width is less than @width
 * */
static void
printnum(void (*putch)(int, void*), void *putdat,
        unsigned long long num, unsigned base, int width, int padc) {
    unsigned long long result = num;
    unsigned mod = do_div(result, base);

    // first recursively print all preceding (more significant) digits
    if (num >= base) {
        printnum(putch, putdat, result, base, width - 1, padc);
    } else {
        // print any needed pad characters before first digit
        while (-- width > 0)
            putch(padc, putdat);
    }
    // then print this (the least significant) digit
    putch("0123456789abcdef"[mod], putdat);
}

/* *
 * ngx_printfmt - format a string and print it by using putch
 * @putch:        specified putch function, print a single character
 * @putdat:        used by @putch function
 * @fmt:        the format string to use
 * */
void
ngx_printfmt(void (*putch)(int, void*), void *putdat, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    ngx_vprintfmt(putch, putdat, fmt, ap);
    va_end(ap);
}

/* sprintbuf is used to save enough information of a buffer */
struct sprintbuf {
    char *buf;   // address pointer points to the first unused memory
    char *ebuf;  // points the end of the buffer
    int cnt;     // the number of characters that have been placed in this buffer
};

/* *
 * sprintputch - 'print' a single character in a buffer
 * @ch:            the character will be printed
 * @b:            the buffer to place the character @ch
 * */
static void
sprintputch(int ch, struct sprintbuf *b) {
    b->cnt ++;
    if (b->buf < b->ebuf) {
        *b->buf ++ = ch;
    }
}

/* *
 * ngx_snprintf - format a string and place it in a buffer
 * @str:        the buffer to place the result into
 * @size:        the size of buffer, including the trailing null space
 * @fmt:        the format string to use
 * */
char*
ngx_snprintf(char *str, size_t size, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    str = ngx_vsnprintf(str, size, fmt, ap);
    va_end(ap);
    return str;
}

char*
ngx_slprintf(char *str, char *last, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    str = ngx_vslprintf(str, last, fmt, ap);
    va_end(ap);
    return str;
}


/* *
 * ngx_vsnprintf - format a string and place it in a buffer, it's called with a va_list
 * instead of a variable number of arguments
 * @str:        the buffer to place the result into
 * @size:        the size of buffer, including the trailing null space
 * @fmt:        the format string to use
 * @ap:            arguments for the format string
 *
 * This function will return pointer that points to the end of text,
 * excluding the trailing '\0'.
 *
 * Call this function if you are already dealing with a va_list.
 * Or you probably want ngx_snprintf() instead.
 * */
char *
ngx_vsnprintf(char *str, size_t size, const char *fmt, va_list ap) {
    struct sprintbuf b = {str, str + size-1 , 0};

    // print the string to the buffer
    ngx_vprintfmt((void*)sprintputch, &b, fmt, ap);
    // null terminate the buffer
    *b.buf = '\0';
	ASSERT( b.buf + b.cnt == b.ebuf );
    return b.ebuf;
}

char *
ngx_vslprintf(char *str, char* last, const char *fmt, va_list ap) {
    struct sprintbuf b = {str, last, 0};

    // print the string to the buffer
    ngx_vprintfmt((void*)sprintputch, &b, fmt, ap);
    // null terminate the buffer
    *b.buf = '\0';
	ASSERT( b.buf + b.cnt == b.ebuf );
    return b.ebuf;
}

/* *
 * ngx_vprintfmt - format a string and print it by using putch, it's called with a va_list
 * instead of a variable number of arguments
 * @putch:        specified putch function, print a single character
 * @putdat:        used by @putch function
 * @fmt:        the format string to use
 * @ap:            arguments for the format string
 *
 * Call this function if you are already dealing with a va_list.
 * Or you probably want ngx_printfmt() instead.
 * */
void
ngx_vprintfmt(void (*putch)(int, void*), void *putdat, const char *fmt, va_list ap) {
    register const char *p;
    register int ch, err;
    unsigned long long num;
    int base, width, precision, lflag, altflag;

    while (1) {
        while ((ch = *(unsigned char *)fmt ++) != '%') {
            if (ch == '\0') {
                return;
            }
            putch(ch, putdat);
        }

        // Process a %-escape sequence
        char padc = ' ';
        width = precision = -1;
        lflag = altflag = 0;

    reswitch:
        switch (ch = *(unsigned char *)fmt ++) {

        // flag to pad on the right
        case '-':
            padc = '-';
            goto reswitch;

        // flag to pad with 0's instead of spaces
        case '0':
            padc = '0';
            goto reswitch;

        // width field
        case '1' ... '9':
            for (precision = 0; ; ++ fmt) {
                precision = precision * 10 + ch - '0';
                ch = *fmt;
                if (ch < '0' || ch > '9') {
                    break;
                }
            }
            goto process_precision;

        case '*':
            precision = va_arg(ap, int);
            goto process_precision;

        case '.':
            if (width < 0)
                width = 0;
            goto reswitch;

        case '#':
            altflag = 1;
            goto reswitch;

        process_precision:
            if (width < 0)
                width = precision, precision = -1;
            goto reswitch;

        // long flag (doubled for long long)
        case 'l':
            lflag ++;
            goto reswitch;

        // character
        case 'c':
            putch(va_arg(ap, int), putdat);
            break;

        // error message
		case 'e':
			err = va_arg(ap, int);
			if (err < 0) {
				err = -err;
			}
			if (err > NGX_MAX_ERROR_STR) {
				ngx_printfmt(putch, putdat, "error %d", err);
			}
			else {
				ngx_printfmt(putch, putdat, "%s", p);
			}
		break;

		// string
		case 's':
            if ((p = va_arg(ap, char *)) == NULL) {
                p = "(null)";
            }
            if (width > 0 && padc != '-') {
                for (width -= strnlen(p, precision); width > 0; width --) {
                    putch(padc, putdat);
                }
            }
            for (; (ch = *p ++) != '\0' && (precision < 0 || -- precision >= 0); width --) {
                if (altflag && (ch < ' ' || ch > '~')) {
                    putch('?', putdat);
                }
                else {
                    putch(ch, putdat);
                }
            }
            for (; width > 0; width --) {
                putch(' ', putdat);
            }
            break;

        // (signed) decimal
        case 'd':
            num = getint(&ap, lflag);
            if ((long long)num < 0) {
                putch('-', putdat);
                num = -(long long)num;
            }
            base = 10;
            goto number;

        // unsigned decimal
        case 'u':
            num = getuint(&ap, lflag);
            base = 10;
            goto number;

        // (unsigned) octal
        case 'o':
            num = getuint(&ap, lflag);
            base = 8;
            goto number;

        // pointer
        case 'p':
            putch('0', putdat);
            putch('x', putdat);
            num = (unsigned long long)va_arg(ap, void *);
            base = 16;
            goto number;

        // (unsigned) hexadecimal
        case 'x':
            num = getuint(&ap, lflag);
            base = 16;
        number:
            printnum(putch, putdat, num, base, width, padc);
            break;

        // escaped '%' character
        case '%':
            putch(ch, putdat);
            break;

        // unrecognized escape sequence - just print it literally
        default:
            putch('%', putdat);
            for (fmt --; fmt[-1] != '%'; fmt --)
                /* do nothing */;
            break;
        }
    }
}

/* *
 * getuint - get an unsigned int of various possible sizes from a varargs list
 * @ap:            a varargs list pointer
 * @lflag:        determines the size of the vararg that @ap points to
 * */
static unsigned long long
getuint(va_list *ap, int lflag) {
    if (lflag >= 2) {
        return va_arg(*ap, unsigned long long);
    }
    else if (lflag) {
        return va_arg(*ap, unsigned long);
    }
    else {
        return va_arg(*ap, unsigned int);
    }
}

/* *
 * getint - same as getuint but signed, we can't use getuint because of sign extension
 * @ap:            a varargs list pointer
 * @lflag:        determines the size of the vararg that @ap points to
 * */
static long long
getint(va_list *ap, int lflag) {
    if (lflag >= 2) {
        return va_arg(*ap, long long);
    }
    else if (lflag) {
        return va_arg(*ap, long);
    }
    else {
        return va_arg(*ap, int);
    }
}

/* panic_spin - print error information and stall
 *	
 * should not be called directly,
 * use PANIC & ASSERT insted
 * */
void panic_spin(char* filename,			\
				int line,				\
				const char* func,		\
				const char* condition)	\
{
	printf("\n\n\n!!!!! error !!!!!\n");
	printf("filename: %s\n", filename);
	printf("line:%d\n",line);
	printf("function: %s\n",(char*)func);
	printf("condition: %s\n",(char*)condition);
	while(1);
}
