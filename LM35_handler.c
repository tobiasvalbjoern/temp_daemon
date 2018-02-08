/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <syslog.h>

volatile sig_atomic_t T;
 
void set_temp(void){
    //get temperature data from LM35
}

float get_temp(void){
    //just a dummy variable
    float x = T;
    return x;
}

// The signal handler reads the temp and re-enables itself
//when receiving interrupt from alarm function every 15 sec
void catch_alarm (int t_sig)
{
    float i = get_temp();
    syslog(LOG_INFO, "Temperature read = %.1f", i);
}

unsigned int set_alarm_sec (unsigned int t_seconds)
{
  if (signal(SIGALRM, catch_alarm) == SIG_ERR) {
		syslog(LOG_ERR,"Can't catch %d",SIGALRM);
	}
    
  struct itimerval old, new;
  
  //This is the regular interval.
  new.it_interval.tv_sec = (long int) t_seconds;
  new.it_interval.tv_usec = 0;
  
  //The timer counts down from this value initially before starting.
  new.it_value.tv_sec = (long int) t_seconds;
  new.it_value.tv_usec = 0;
  
  if (setitimer (ITIMER_REAL, &new, &old) < 0){
      syslog(LOG_NOTICE, "Could not install timer\n");
      return 0;
  }
  else
    return old.it_value.tv_sec;
}