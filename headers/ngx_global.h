#ifndef __NGX_GBLDEF_H__
#define __NGX_GBLDEF_H__

#include <signal.h>
#include "ngx_log.h"

/* Global Varaibles */
//defined in nginx.cpp
extern pid_t	ngx_pid;		//Process ID of current process
extern pid_t	ngx_parent;		//Process ID of parent process
extern int		ngx_process;	//Indicate type of Process: Master, Slave
//defined in ngx_log.cpp
extern ngx_log_t ngx_log;		//Log level and output file descriptor

#endif  //__NGX_GBLDEF_H__

