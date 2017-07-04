/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NotSupportedException.h
 * Author: serg
 *
 * Created on April 13, 2017, 10:25 PM
 */

#ifndef NOTSUPPORTEDEXCEPTION_H
#define NOTSUPPORTEDEXCEPTION_H
#include "CHDLC_Channel.h"

class CHDLC_Channel::CNotSupportedException : public std::exception {
public:
    CNotSupportedException(std::string Description){
        std::cout<<"CNotSupportedException: "<<Description<<std::endl;
    }
};

#endif /* NOTSUPPORTEDEXCEPTION_H */

