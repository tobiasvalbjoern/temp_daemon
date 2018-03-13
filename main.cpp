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


string cmd_handler(string input) {
    string output("");

    if (input == string("GET TEMP")) {
        float t = LM35_handler_get_temp();
        output = "REPLY TEMP: " + to_string(t) + "\n";
    } else if (input == string("HEAT ON")) {
        syslog(LOG_INFO, "Turning on heat...");
        LM35_handler_set_heat(true);
    } else if (input == string("HEAT OFF")) {
        syslog(LOG_INFO, "Turning off");
        LM35_handler_set_heat(false);
    } else {
        syslog(LOG_WARNING, "Unknown command");
        output = "Unknown command\n";
    }

    return output;
}



int main(int argc, char* argv[]) {
    char json[] = "{\"jsonrcp\": \"2.0\", \"method\": \"getTemp\", \"params\": [], \"id\": \"1\"}";
    char json_array[] = "{\"jsonrcp\": \"2.0\", \"method\": \"getTemp\", \"params\": [3,4], \"id\": \"1\"}";
    char json_object[] = "{\"jsonrcp\": \"2.0\", \"method\": \"getTemp\", \"params\": {\"unit\": \"c\"}, \"id\": \"1\"}";
    char json_noid[] = "{\"jsonrcp\": \"2.0\", \"method\": \"getTemp\", \"params\": {\"unit\": \"c\"}}";

    jsonrpc_debug(json);
    jsonrpc_debug(json_array);
    jsonrpc_debug(json_object);
    jsonrpc_debug(json_noid);    
    
    daemon_init("temp_daemon", true); //Start as daemon with debugging enabled
    tserver_init(INTERFACE, PORT, jsonrpc_handler); //Starts a new threaded server

    //Initialize the LM35 temperature sensor (Update interval = 1 secs)
    LM35_handler_init(1);

    while (1) { //Here you can implement some main logic...
        sleep(1);
        //syslog(LOG_INFO, "Logging...");
    }

    daemon_stop(EXIT_SUCCESS); // Stop the daemon
}