#include <stdio.h>
#include <unistd.h>

#include "ngx_macro.h"
#include "ngx_conf.h"

static void ngx_worker_start(int threads);
static void ngx_worker_init(int threads);
static void ngx_worker_cycle(int);

static void ngx_master_cycle(int proc_num, const char* proc_name);
static void ngx_process_spawn(int threads, const char* proc_name);


/* ngx_master_cycle -  Master Main Loop
 *
 * @proc_num: identification number (master or slave)
 * @proc_name: TODO: change process name due to this 
 *
 * This function will return when nginx server die, normally
 * this would not happen
 *
 * This function is the entrance of both master and slave
 * */
void ngx_master_cycle() {


	/**************  **************
	 * Create worker processes
	 * TODO: Read how many worker process need to be create from configure file
	 * *****************************/ 
	NGX_Config *p_config = NGX_Config::GetInstance();
	int workers = 4 ;//p_config-> Not implement yet
	ngx_worker_start(workers);


	/**************  **************
	 * The Main Loop
	 * *****************************/ 
	while(1) {
		usleep(100000);
		printf("Master\n");
	}
}

/* ngx_worker_start - create given numbers of worker
 *
 * @threads: how many worker threads need to be created
 *
 * For Master Process:
 * 	return -1 when fork() failed,
 * 	return process(PID) 
 *
 * For Worker Process:
 * 	this function should not return
 * 
 * */

static int  ngx_worker_start(int threads) {

	for (int i = 0; i < threads; i++) {
		pid_t pid = fork();
		switch (pid)	// generate child process
		{
		case -1:
			printf("Fork Failed!\n");
			return -1;

		case 0:		//Child Process
			ngx_parent = ngx_pid;	//
			ngx_pid = getpid();
			/* Enter Main Loop of Child Process */
			ngx_worker_cycle(i, "Worker");
			break;		//this should not happen

		default:	//Parent Process
			break;	//Go back

		} //switch

	} //for

	return pid;
}

/* ngx_worker_cycle - Main Loop of Slave
 *
 * This function should not return
 *
 * */
void ngx_worker_cycle(int proc_num, const char* proc_name) {
	
}

/* ngx_worker_init - Initialization Work for worker process
 *
 * */
static void ngx_worker_init(int proc_num) {

	printf("Initialization\n");
}

