/**
 ============================================================================
 Name        : daemon.c
 Author      : Tobias & Jens
 Version     : 1.0
 Copyright   : None
 Date 		 : 30/1-2018
 Description : Change the process to a daemon and prepares syslog
 ============================================================================
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#include "daemon.h"


//Sig handler for termination of program

void sig_handler(int t_signo) {
	if (t_signo == SIGHUP) {
		syslog(LOG_INFO, "Received %d. Aborting.", SIGHUP);
		daemon_stop(0);
	}
}


/**
 * Turn this process into a daemon and detach stdin, stdout and stderr
 * Starts a syslog session with the given process name
 * Set up to terminate when receiving sighup.
 * Set up for signal handling for timer to read temperature at 15 sec.  
 */
void daemon_init(char *t_process_name) {
	pid_t process_id = 0;
	pid_t sid = 0;
        
	openlog(t_process_name, 0, LOG_USER);
        
	// Create child process
	process_id = fork();
	// Indication of fork() failure
	if (process_id < 0) {
		syslog(LOG_INFO, "fork failed!");
		// Return failure in exit status
		exit(1);
	}
        
	// PARENT PROCESS. Need to kill it.
	if (process_id > 0) {
		syslog(LOG_INFO, "process_id of child process %d", process_id);
		printf("Daemon started: PID %d\n", process_id);
		exit(0);
	}
        
	//unmask the file mode
	umask(0);
	//set new session
	sid = setsid();
	if (sid < 0) {
                syslog(LOG_ERR,"Can't run a program in a new session\n");
		// Return failure
		daemon_stop(1);
	}
        
	// Change the current working directory to root.
	chdir("/");

	// Close stdin. stdout and stderr
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	syslog(LOG_INFO, "STDIN, STDOUT, and STDERR closed.");

	if (signal(SIGHUP, sig_handler) == SIG_ERR) {
		syslog(LOG_ERR,"Can't catch %d",SIGHUP);
	}
} 

void daemon_stop(unsigned int t_exit_code) {
	syslog(LOG_INFO, "Stopping daemon");
	syslog(LOG_INFO, "Exit code: %u", t_exit_code);
	closelog();
	exit(t_exit_code);
}
