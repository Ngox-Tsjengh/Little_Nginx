#ifndef __NGX_LOG_H__
#define __NGX_LOG_H__

typedef struct
{
	int    log_level;   //Nine Levels of error
	int    fd;          //Where to output
}ngx_log_t;

/* ngx_cpymem - copy memory from src to dst
 *
 * !!! this function will return a pointer that
 * points to destination + copied length, it's 
 * convinent when coping multiple bytes
 *
 * @dst:		destination
 * @src:		source 
 * 
 * return destination + copied length
 * */ 
#define ngx_cpymem(dst, src, n)   (((char *) memcpy(dst, src, n)) + (n))  

#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))              

void ngx_log_stderr(int err, const char *fmt, ...);
char *ngx_log_errno(char *buf, char *last, int err);
void ngx_log_error_core(int level,  int err, const char *fmt, ...);
void ngx_log_init();

#endif //__NGX_LOG_H__
