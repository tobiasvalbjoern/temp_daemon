/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <syslog.h>

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<stdlib.h>
using namespace std;

#define LDR_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"


volatile sig_atomic_t T;

int read_analog(int number){ // returns the input as an int
   stringstream ss;
   ss << LDR_PATH << number << "_raw";
   fstream fs;
   fs.open(ss.str().c_str(), fstream::in);
   fs >> number;
   fs.close();
   return number;
}

float read_temp(void){
    
   float cur_voltage = read_analog(0) * (1.80f/4096.0f);
   return (100*cur_voltage);

}

float get_temp(void){return T;}

// The signal handler reads the temp and re-enables itself
//when receiving interrupt from alarm function every 15 sec
void catch_alarm (int t_sig)
{
    T = read_temp();
    syslog(LOG_INFO, "Temperature read = %.1f", T);
}

unsigned int set_alarm_sec (unsigned int t_seconds)
{
  if (signal(SIGALRM, catch_alarm) == SIG_ERR) {
		syslog(LOG_ERR,"Can't catch %d",SIGALRM);
	}
    
  struct itimerval timer;
  
  //This is the regular interval.
  timer.it_interval.tv_sec = (long int) t_seconds;
  timer.it_interval.tv_usec = 0;
  
  //The timer counts down from this value initially before starting.
  timer.it_value.tv_sec = (long int) t_seconds;
  timer.it_value.tv_usec = 0;
  
  if (setitimer (ITIMER_REAL, &timer, NULL) < 0){
      syslog(LOG_NOTICE, "Could not install timer\n");
      return 0;
  }
  else
    return NULL;
}