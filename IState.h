/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IState.h
 * Author: serg
 *
 * Created on April 11, 2017, 11:21 PM
 */

#ifndef ISTATE_H
#define ISTATE_H
#include "CHDLC_Channel.h"
#include "CFrame.h"

class CHDLC_Channel::IState {
public:
    virtual ~IState();
    
    virtual void disconnect(Channel_t Channel);

    virtual bool receiveFrame(Frame_t Frame);

    virtual bool transmitBuffer(Channel_t Channel, HDLC_Buffer_t Buffer);

protected:
        IState();  
    };
        

#endif /* ISTATE_H */

