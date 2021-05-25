#ifndef __NGX_STRING_H_
#define __NGX_STRING_H_

#include <stdarg.h>		//va_list
#include <stddef.h>		//size_t

void   Rtrim(char *string);
void   Ltrim(char *string);

void ngx_printfmt(void (*putch)(int, void *), void *putdat, const char *fmt, ...);
void ngx_vprintfmt(void (*putch)(int, void *), void *putdat, const char *fmt, va_list ap);
char* ngx_snprintf(char *str, size_t size, const char *fmt, ...);
char* ngx_slprintf(char *str, char* last, const char *fmt, ...);
char* ngx_vsnprintf(char *str, size_t size, const char *fmt, va_list ap);
char* ngx_vslprintf(char *str, char* last, const char *fmt, va_list ap);


//PANIC and ASSERT, merge from ngx_debug.c
void panic_spin(char* filename, int line, const char* func, const char* condition);

#define PANIC(...) panic_spin(__FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef NDEBUG
	#define ASSERT(CONDITION) ((void)0)
#else
#define ASSERT(CONDITION) 	\
	if(CONDITION){}else{	\
		PANIC(#CONDITION);	\
	}
#endif //NDEBUG


#endif //__NGX_STRING_H_
