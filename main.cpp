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
#include <cstdlib>
#include <string>
#include <iostream>

#include "daemon.h"
#include "tserver.h"
#include "LM35_handler.h"

#define PORT "1955" 

using namespace std;

string cmd_handler(string input) {
    cout << "Handler got: #" << input << "#" << endl;
    string output("");
    
    if(input == string("GET TEMP")) {
        float t = LM35_handler_get_temp();
        output = "REPLY TEMP: " + to_string(t) + "\n";
    } else {
        output = "Unknown command\n";
    }
    
    return output;
} 

int main(int argc, char* argv[]) {
	daemon_init("temp_daemon"); //Start as daemon
	tserver_init("localhost", PORT, cmd_handler); //Starts a new threaded server
        
        //Initialize the LM35 temperature sensor (Update interval = 15 secs)
        LM35_handler_init(15);
	
        while (1) { //Here you can implement some main logic...
		sleep(1);
		//syslog(LOG_INFO, "Logging...\n");
	}

	daemon_stop(EXIT_SUCCESS); // Stop the daemon
}