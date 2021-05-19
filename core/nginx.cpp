#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "ngx_macro.h"
#include "ngx_conf.h"

#include "ngx_main.h"

/* Global Varaibles */
pid_t	ngx_pid;		//Process ID of current process
pid_t	ngx_parent;		//Process ID of parent process
int		ngx_process;	//Indicate type of Process: Master, Slave

void source_free();

int main(int argc, char* const *argv) {

	/**  0. Preparation
	 */
	ngx_pid		= getpid();	//Get Process ID
	ngx_parent	= getppid();
	ngx_process = NGX_PROCESS_MATSER;

	/* 1. Initialization
	 */
	NGX_Config *p_config = NGX_Config::GetInstance();	//Singelton
	if (p_config->Load("nginx.conf") == false) {
		exit(2);
	}
	

	/* Create Daemon
	 */

	/* Main Loop
	 * */
	ngx_master_cycle();

	source_free();

	return 0;	//Indicates Normal Exit
}

/**  
 * Free all source before exit 
 *
 * # No parameter
 * # No return value
 * */
void source_free() {	
 
}
