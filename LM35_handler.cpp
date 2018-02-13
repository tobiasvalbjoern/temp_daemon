/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <syslog.h>
#include <pthread.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<stdlib.h>
using namespace std;

#define LDR_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"

pthread_mutex_t lock;

volatile float T;

int read_analog(int number) { // returns the input as an int
    stringstream ss;
    ss << LDR_PATH << number << "_raw";
    fstream fs;
    fs.open(ss.str().c_str(), fstream::in);
    fs >> number;
    fs.close();
    return number;
}

float read_temp(void) {

    float cur_voltage = read_analog(0) * (1.80f / 4096.0f);
    return (100 * cur_voltage);

}

// The signal handler reads the temp and re-enables itself
//when receiving interrupt from alarm function every 15 sec

void catch_alarm(int t_sig) {
    float t = read_temp();
    //try to lock. Returns 0 when succesfulll
    if (pthread_mutex_lock(&lock) == 0) {
        T = t;
    }

    //the lock is only for adjusting the queue.
    pthread_mutex_unlock(&lock);


    syslog(LOG_INFO, "Temperature read = %.1f", T);
}

float LM35_handler_get_temp(void) {
    float t;
    //try to lock. Returns 0 when succesfulll
    if (pthread_mutex_lock(&lock) == 0) {
        t = T;
    }

    //the lock is only for adjusting the queue.
    pthread_mutex_unlock(&lock);


    return t;
}

void LM35_handler_init(unsigned int t_seconds) {
    if (signal(SIGALRM, catch_alarm) == SIG_ERR) {
        syslog(LOG_ERR, "Can't catch %d", SIGALRM);
    }

    struct itimerval timer;
    // Initialise the lock
    pthread_mutex_init(&lock, NULL);


    //This is the regular interval.
    timer.it_interval.tv_sec = (long int) t_seconds;
    timer.it_interval.tv_usec = 0;

    //The timer counts down from this value initially before starting.
    timer.it_value.tv_sec = 0;
    //This needs to have a value besides zero.
    timer.it_value.tv_usec = 1000;

    if (setitimer(ITIMER_REAL, &timer, NULL) < 0) {
        syslog(LOG_NOTICE, "Could not install timer\n");
        return;
    } else
        return;
}