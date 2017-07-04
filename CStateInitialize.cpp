/* 
 * File:   InitializeState.cpp
 * Author: serg
 * 
 * Created on April 13, 2017, 11:26 PM
 */

#include "CStateInitialize.h"
#include "CFrame.h"

//------------------------------------------------------------------------------

CHDLC_Channel::CStateInitialize::CStateInitialize():
    ChannelSettings_(nullptr)
{}

//------------------------------------------------------------------------------

CHDLC_Channel::CStateInitialize::~CStateInitialize() {}

//------------------------------------------------------------------------------

CHDLC_Channel::CStateInitialize* CHDLC_Channel::CStateInitialize::getInstance() {
    static CStateInitialize instance;
    return &instance;
}

//------------------------------------------------------------------------------

void CHDLC_Channel::CStateInitialize::initialize(CHDLC_Channel::SChannelSettings* ChannelSettings){
    ChannelSettings_ = ChannelSettings;
}

//------------------------------------------------------------------------------

bool CHDLC_Channel::CStateInitialize::receiveFrame(Frame_t Frame) {
    
    HDLC_Buffer_t Buffer;
    
    if(Frame->getFormat() == FORMAT_U) {
        CStateInitialize::ChannelSettings_->thisChannel->setMode(Frame->getFormat_U());
        Frame->transmitBuffer(S_FORMAT_RR, CHECKPOINT_UNDEFINED, 0, Buffer);
    } else
        return false;
}

//------------------------------------------------------------------------------


