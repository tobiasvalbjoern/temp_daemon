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

#ifndef LM35_HANDLER_H
#define LM35_HANDLER_H

    float LM35_handler_get_temp(void);
    void LM35_handler_init(unsigned int t_seconds);
    void LM35_handler_set_heat(bool state);

#endif /* LM35_HANDLER_H */

