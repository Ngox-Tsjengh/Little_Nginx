#include <stdarg.h>		//va_list...
#include <fcntl.h>		//open
#include <unistd.h>
#include <sys/time.h>	//gettimeofday
#include <stdio.h>

#include "ngx_conf.h"
#include "ngx_global.h"
#include "ngx_macro.h"
#include "ngx_log.h"

extern "C" {
#include "ngx_string.h"
}


/* err_levels: Level of logs
 *
 * Level of logs can be set in configuration file
 *
 * Level 0~1 will be printed to stderr
 * Other level will be printed to file
 *
 * */
static unsigned char err_levels[][32]  = 
{
    {"stderr"},    //0
    {"emerg"},     //1
    {"alert"},     //2
    {"crit"},      //3
    {"error"},     //4
    {"warn"},      //5
    {"notice"},    //6
    {"info"},      //7
    {"debug"}      //8
};

/* 
 * level & output file descriptor
 * */
ngx_log_t   ngx_log;


/* *
 * ngx_log_stderr - format a string and print it to stderr
 * it supports Variable length arguments 
 * 
 * @err:		error number (not level)
 * @fmt:        the format string to use
 *
 * Call this function if you want print the information to standard error
 * 
 * */
void ngx_log_stderr(int err, const char *fmt, ...)
{
    va_list args;                        	//Variable length arguments list
    char  errstr[NGX_MAX_ERROR_STR+1]; 	//2048, defined in ngx_log.h
    char  *p;								//current write position

    memset(errstr,0,sizeof(errstr));     	//initialisation as 0

    char *last = errstr + NGX_MAX_ERROR_STR;  //the last of errstr
                                              
    p = ngx_cpymem(errstr, "nginx: ", 7);     //p points after nginx:

    va_start(args, fmt); 
    p = ngx_vslprintf(p,last,fmt,args); 
    va_end(args);

    if (err) { p = ngx_log_errno(p, last, err); }

    //add linefeed to the end
    if (p >= (last - 1)) { p = (last - 1) - 1; }
    *p++ = '\n'; 

    //write to standard error
    write(STDERR_FILENO,errstr,p - errstr);

    if(ngx_log.fd > STDERR_FILENO) //means that error log file exists and it's no console
    {
        err = 0;   	//avoid repetition 
        p--;*p = 0; //delete linefeed, as it'll be added in ngx_log_error_core
        ngx_log_error_core(NGX_LOG_STDERR,err,(const char *)errstr);
    }
    return;
}


/* *
 * ngx_log_errno - get error information from error number
 * 
 * @buf:		string buffer 
 * @last:		last position in string buffer
 * @err:		error number (not level)
 *
 * This function will get error information and print it to buf
 * */
char *ngx_log_errno(char *buf, char *last, int err)
{
    char *perrorinfo = strerror(err);	//won't return NULL
    size_t len = strlen(perrorinfo);

	/* add extra string to buf */
    char leftstr[10] = {0};
    sprintf(leftstr," (%d: ",err);
    size_t leftlen = strlen(leftstr);

    char rightstr[] = ") ";
    size_t rightlen = strlen(rightstr);

	/* copy error information string to buffer */
  	size_t extralen = leftlen + rightlen; 
    if ((buf + len + extralen) < last)
    {
        buf = ngx_cpymem(buf, leftstr, leftlen);
        buf = ngx_cpymem(buf, perrorinfo, len);
        buf = ngx_cpymem(buf, rightstr, rightlen);
    }
    return buf;
}


/* *
 * ngx_log_gettime - get time 
 * 
 * This function will return a tm* struct pointer, 
 * which stores the time
 * */
static inline tm ngx_gettime()
{
	struct timeval   tv;
    struct tm        tm;
    time_t           sec;   
    memset(&tv,0,sizeof(struct timeval));
    memset(&tm,0,sizeof(struct tm));

    gettimeofday(&tv, NULL);     
    sec = tv.tv_sec;            

    localtime_r(&sec, &tm);      //transform to local time and save to tm
    tm.tm_mon++;                 
    tm.tm_year += 1900;         

	return tm;
}

/* *
 * ngx_log_error_core - format a string and print it to error log
 * it supports Variable length arguments 
 * don't need to add linefeed to end of line
 * 
 * @level:		error level: 0~8
 * @err:		error number
 * @fmt:        the format string to use
 *
 * Call this function if you want print the information to error log
 * 
 * */
void ngx_log_error_core(int level,  int err, const char *fmt, ...)
{
    char  *last;
    char  errstr[NGX_MAX_ERROR_STR+1];   

    memset(errstr,0,sizeof(errstr));
    last = errstr + NGX_MAX_ERROR_STR;

    char      *p;    //current write position (errstr)
    va_list   args;
	
	struct tm  time = ngx_gettime(); 

	/* Format the time to string */
    char strcurrtime[64] = {0};  
    ngx_slprintf(strcurrtime,
            	(char *)-1, 
                "%4d/%02d/%02d %02d:%02d:%02d",     /* year/month/day hour:minute:second */
                time.tm_year,	time.tm_mon,	 time.tm_mday,	time.tm_hour,	time.tm_min, 	time.tm_sec
			);
    p = ngx_cpymem(errstr,strcurrtime,strlen((const char *)strcurrtime));  //add time to errstr:  2021/05/24 20:26:07
    p = ngx_slprintf(p, last, " [%s] ", err_levels[level]);                //add error level:	  2021/05/24 20:26:07 [emerg]
    p = ngx_slprintf(p, last, "%P: ",ngx_pid);                             //add process ID:	  2021/05/24 20:26:07 [emerg] 2037:

    va_start(args, fmt);                     
    p = ngx_vslprintf(p, last, fmt, args);   
    va_end(args);                            

    if (err) { p = ngx_log_errno(p, last, err); }
    
    if (p >= (last - 1)) { p = (last - 1) - 1; }
    *p++ = '\n';

    while(1)
    {
        if (level > ngx_log.log_level) { //the error level is too low to print 
            break;	//don't write
        }

        //write to file
        ssize_t n = write(ngx_log.fd,errstr,p - errstr);  
		if (n == -1)
		{ //write to file failed, output to console
			if(ngx_log.fd != STDERR_FILENO) {
				n = write(STDERR_FILENO,errstr,p - errstr);
			}
        }
        break;
    }//end while
    return;
} 

//Log Initialization 
void ngx_log_init()
{
    char *plogname = NULL;

    /* Read Configuration */
    NGX_Config *p_config = NGX_Config::GetInstance();
	//1. Log file name
    plogname = (char *)p_config->GetString("Log");
    if(plogname == NULL) { plogname = (char *) NGX_ERROR_LOG_DEFAULT;} //errors.log, default name
	//2. Log level
    ngx_log.log_level = p_config->GetInt("LogLevel",NGX_LOG_NOTICE);//set log level, default is notice(6)

	/* Open log file to write */
    ngx_log.fd = open((const char *)plogname,O_WRONLY|O_APPEND|O_CREAT,0644);  
    if (ngx_log.fd == -1)  //error handling
    {
        ngx_log_stderr(errno,"[alert] could not open error log file: open() \"%s\" failed", plogname);
        ngx_log.fd = STDERR_FILENO; //2, console
    } 
    return;
}

