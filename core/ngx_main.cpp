#include <stdio.h>
#include <unistd.h>

#include "ngx_macro.h"
#include "ngx_conf.h"
#include "ngx_global.h"

extern "C" {
#include "ngx_string.h"
}

void ngx_master_cycle();

static int ngx_worker_start(int threads);
static void ngx_worker_init(int threads);
static void ngx_worker_cycle(int proc_num, const char *proc_name);

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
	int workers = p_config->GetInt("WorkerProcesses",1); //Read from Configuration, if fail, the default value is 1
	ngx_worker_start(workers);

	/**************  **************
	 * The Main Loop
	 * *****************************/ 
	while(1) {
		usleep(1000000);
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
static int  ngx_worker_start(int threads)
{
	pid_t pid;
	for (int i = 0; i < threads; i++) {
		pid = fork();
		switch (pid)	// generate child process
		{
		case -1:
			printf("Fork Failed!\n");
			return -1;

		case 0:	//Child Process
			ngx_parent = ngx_pid;	//Global 
			ngx_pid = getpid();		//Global
			/* Enter Main Loop of Child Process */
			ngx_worker_cycle(i, "Worker");
			PANIC("workers reach place they shouldn't reach");
			break;		//this should not happen

		default://Parent Process
			break;

		} //switc

	} //for
	return pid;
}

/* ngx_worker_cycle - Main Loop of Slave
 *
 * @proc_num: the identification number of process
 * @proc_name: should be "Worker"
 *
 * This function should not return
 * */
void ngx_worker_cycle(int proc_num, const char* proc_name) {
	ngx_worker_init(proc_num);

	while(1) {
		usleep(1000000);
		printf("I'm %s %d\n",proc_name, proc_num);
	}
}

/* ngx_worker_init - Initialization Work for worker process
 *
 * @proc_num: the identification number of process
 * */
static void ngx_worker_init(int proc_num) {
	ngx_process = NGX_PROCESS_WORKER;	//set type of process

	printf("Initialization %d\n",proc_num);

	return;
}

