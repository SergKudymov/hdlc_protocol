/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IState.cpp
 * Author: serg
 * 
 * Created on April 11, 2017, 11:21 PM
 */

#include "IState.h"
#include "CNotSupportedException.h"

CHDLC_Channel::IState::IState() {
}


CHDLC_Channel::IState::~IState() {
}


void CHDLC_Channel::IState::disconnect(Channel_t channel){
    //channel->changeState(CStateDisconnect::getInstance());
}


bool CHDLC_Channel::IState::receiveFrame(Frame_t Frame){
    throw CNotSupportedException("CHDLC_Channel::IState::receiveFrame");
}

bool CHDLC_Channel::IState::transmitBuffer(Channel_t Channel, HDLC_Buffer_t Buffer){
    throw CNotSupportedException("CHDLC_Channel::IState::transmitBuffer");
}


