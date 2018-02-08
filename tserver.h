/*
 ============================================================================
 Name        : tserver.h
 Author      : BEEJ's guide to network programming, Tobias, Jens
 Version     : 2.0
 Copyright   : None
 Date 		 : 29/5-2017
 Modified    : 30/1-2018
 Description : This tserver program will accept multiple incoming connections
               and they will echo back the string send from the client.
 ============================================================================
 *
 */

#ifndef TSERVER_H_
#define TSERVER_H_

#ifdef __cplusplus
extern "C" {
#endif
    
void tserver_init(char * interface, char * listen_port);

#ifdef __cplusplus
}
#endif

#endif /* TSERVER_H_ */
