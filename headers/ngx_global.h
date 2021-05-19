#ifndef __NGX_GBLDEF_H__
#define __NGX_GBLDEF_H__

#include <signal.h>

/* Global Varaibles */
extern pid_t	ngx_pid;		//Process ID of current process
extern pid_t	ngx_parent;		//Process ID of parent process
extern int		ngx_process;	//Indicate type of Process: Master, Slave

#endif  //__NGX_GBLDEF_H__
