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
#include "jsonrpc.h"

#define INTERFACE "0.0.0.0" //0.0.0.0 - Listening on all interfaces
#define PORT "1955" 

using namespace std;

int main(int argc, char* argv[]) {
    /*
    char json[] = "{\"jsonrpc\": \"2.0\", \"method\": \"getTemp\", \"params\": [], \"id\": \"1\"}";
    char json_array[] = "{\"jsonrpc\": \"2.0\", \"method\": \"getTemp\", \"params\": [3,4], \"id\": \"1\"}";
    char json_object[] = "{\"jsonrpc\": \"2.0\", \"method\": \"getTemp\", \"params\": {\"unit\": \"c\"}, \"id\": \"1\"}";
    char json_noid[] = "{\"jsonrpc\": \"2.0\", \"method\": \"getTemp\", \"params\": {\"unit\": \"c\"}}";

    jsonrpc_debug(json);
    jsonrpc_debug(json_array);
    jsonrpc_debug(json_object);
    jsonrpc_debug(json_noid);    
    */
    
    daemon_init("temp_daemon", true); //Start as daemon with debugging enabled
    
    //Request - {"jsonrcp": "2.0", "method": "getTemp", "id": "1"} 
    tserver_init(INTERFACE, PORT, jsonrpc_handler); //Starts a new threaded server

    //Initialize the LM35 temperature sensor (Update interval = 1 secs)
    LM35_handler_init(1);

    while (1) { //Here you can implement some main logic...
        sleep(1);
        //syslog(LOG_INFO, "Logging...");
    }

    daemon_stop(EXIT_SUCCESS); // Stop the daemon
}