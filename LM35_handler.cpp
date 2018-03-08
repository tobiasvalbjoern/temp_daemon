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
#include<syslog.h>

using namespace std;

#define ABSOLUTE_ZERO -273.15f;
#define ADC_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"

pthread_mutex_t lock;

volatile float T = ABSOLUTE_ZERO;

/**
 * Read an ADC pin from a beaglebone black
 * @param number ADC pin to read 
 * @return ADC numeric value 10 bit. On error -1
 */
int read_analog(int number) { // returns the input as an int
    stringstream ss;
    ss << ADC_PATH << number << "_raw";
    fstream fs;
    fs.open(ss.str().c_str(), fstream::in);
    if(fs.is_open()) {
        fs >> number;
        fs.close();
    } else {
        number =  -1; 
    }
    return number;
}

float read_temp(void) {
    //read from analog0 on pin 39
    int analog_value = read_analog(0);
    
    if(analog_value > 0) {
        float cur_voltage = read_analog(0) * (1.80f / 4096.0f);
        return (100 * cur_voltage);
    } else {
        syslog(LOG_WARNING, "Could not read ADC. Faking value");
        return rand() % 50; //Return a random temperature
    }
}

/**
 * Alarm handler. Catches periodic alarm and performs an ADC conversion
 * Thread safe.
 * @param t_sig
 */
void catch_alarm(int t_sig) {
    float t = read_temp();
    //try to lock. Returns 0 when succesfulll
    if (pthread_mutex_lock(&lock) == 0) {
        T = t;
    }

    //the lock is only for adjusting the static value.
    pthread_mutex_unlock(&lock);


    syslog(LOG_INFO, "Temperature read = %.1f", T);
}


/**
 * Read the last recorded temperature from the LM35 sensor
 * 
 * Call LM35_handler_init before using this routine!
 * 
 * @return Temperature in celcius
 */
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

/**
 * Configure the LM35 temperature sensor
 * 
 * This functions configures reading of temperature from an LM35 sensor
 * connected to an analog pin. The value is updated periodically using the linux
 * SIGALARM feature.
 * @param t_seconds Update interval in seconds
 */

/**
 * Helper write function that writes a single string value to a file in the path provided
 * @param path The sysfs path of the file to be modified
 * @param filename The file to be written to in that path
 * @param value The value to be written to the file
 * @return
 */
int write(string path, string filename, string value){
   ofstream fs;
   fs.open((path + filename).c_str());
   if (!fs.is_open()){
	   syslog(LOG_ERR, "GPIO: write failed to open file ");
           return -1;
   }
   fs << value;
   fs.close();
   return 0;
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
    }
    
    system("config-pin p8.41 out");
    //P8.20 as GPIO/output
    write("/sys/class/gpio/gpio20/", "direction", "out"); 
}

void LM35_handler_set_heat(bool state){
    //Turn on GPIO_20 on pin 41.
    if(state) {
        write("/sys/class/gpio/gpio20/", "value", "1");   
    } else {
        write("/sys/class/gpio/gpio20/", "value", "0");   
    }

}