#ifndef __NGX_MACRO_H__
#define __NGX_MACRO_H__


typedef unsigned char u_char ;
//#define NULL ((void*) 0)



/* Process Related
 * */
#define	NGX_PROCESS_MATSER 		0 	//Master, Manage worker, high privilege
#define NGX_PROCESS_WORKER		1	//Worker, do real work, low privilege


/* Log Related*/
#define ERROR_LEVEL 			9
//Nine Levels of error
#define NGX_LOG_STDERR            0    //
#define NGX_LOG_EMERG             1    //
#define NGX_LOG_ALERT             2    //
#define NGX_LOG_CRIT              3    //
#define NGX_LOG_ERR               4    //
#define NGX_LOG_WARN              5    //
#define NGX_LOG_NOTICE            6    //
#define NGX_LOG_INFO              7    //
#define NGX_LOG_DEBUG             8    //
//Maximum length of error string
#define NGX_MAX_ERROR_STR 		2048

/* Default place to save error log */
#define NGX_ERROR_LOG_DEFAULT	"errors.log"


#define do_div(n, base)                              \
    ({                                               \
        int __res;                                   \
        __res = ((unsigned long)n) % (unsigned)base; \
        n = ((unsigned long)n) / (unsigned)base;     \
        __res;                                       \
    })


#endif /* __NGX_MACRO_H__ */
