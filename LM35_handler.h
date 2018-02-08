/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   get_temp.h
 * Author: tobias
 *
 * Created on February 6, 2018, 11:08 AM
 */

#ifndef GET_TEMP_H
#define GET_TEMP_H

#ifdef __cplusplus
extern "C" {
#endif

    void set_temp(void);
    float get_temp(void);
    unsigned int set_alarm_sec(unsigned int t_seconds);
 
#ifdef __cplusplus
}
#endif

#endif /* GET_TEMP_H */

