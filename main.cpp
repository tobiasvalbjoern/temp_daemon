/**
 ============================================================================
 Name        : main.c
 Author      : Tobias & Jens
 Version     : 1.0
 Copyright   : None
 Date 		 : 30/1-2018
 Description : Starts as a daemon and opens a multithreaded server.
 ============================================================================
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <syslog.h>


#include "daemon.h"
#include "tserver.h"
#include "LM35_handler.h"

#define PORT "1955" 

int main(int argc, char* argv[]) {
	daemon_init("temp_daemon");
	tserver_init("localhost", PORT);
        //A timer function that will create a signal interrupt every 15 sec
        LM35_handler_init(15);
	while (1) {
		sleep(1);
		//syslog(LOG_INFO, "Logging...\n");
	}

	daemon_stop(0);

}
