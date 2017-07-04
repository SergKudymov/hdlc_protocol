/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CStateDisconnect.h
 * Author: serg
 *
 * Created on April 11, 2017, 11:25 PM
 */

#ifndef DISCONNECTSTATE_H
#define DISCONNECTSTATE_H

#include "CHDLC_Channel.h"
#include "IState.h"

class CHDLC_Channel::CStateDisconnect: public CHDLC_Channel::IState {
public:
    CStateDisconnect();
    CStateDisconnect(const CStateDisconnect& orig);
    virtual ~CStateDisconnect();
private:

};

#endif /* DISCONNECTSTATE_H */

