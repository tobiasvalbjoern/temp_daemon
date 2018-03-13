/**
 ============================================================================
 Name        : daemon.h
 Author      : Tobias & Jens
 Version     : 1.0
 Copyright   : None
 Date 		 : 30/1-2018
 Description : Change the process to a daemon and prepares syslog
 ============================================================================
 *
 */

#ifndef DAEMON_H_
#define DAEMON_H_

#ifdef __cplusplus
extern "C" {
#endif

void daemon_init(char *process_name, int debug);
void daemon_stop(unsigned int exit_code);

#ifdef __cplusplus
}
#endif
#endif /* DAEMON_H_ */
