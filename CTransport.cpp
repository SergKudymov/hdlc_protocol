/* 
 * File:   HDLC_transport.cpp
 * Author: serg
 * 
 * Created on April 9, 2017, 8:02 PM
 */

#include "CTransport.h"
#include "CFrame.h"
#include <stdint.h>

const uint8_t  CHDLC_Channel::CTransport::HDLC_FLG =            0x7e;
const uint8_t CHDLC_Channel::CTransport::B_STUF_ESC_SEQ =       0x7d;
const uint8_t CHDLC_Channel::CTransport::B_STUF_FLG =           0x5e;
const uint8_t CHDLC_Channel::CTransport::B_STUF_ESC =           0x5d;

//------------------------------------------------------------------------------

CHDLC_Channel::CTransport::CTransport(SChannelSettings* ChannelSettings):
            ChannelSettings_(ChannelSettings),
            ByteStuffingFlag_(false)
{}

//------------------------------------------------------------------------------

CHDLC_Channel::CTransport::~CTransport() {
}

//------------------------------------------------------------------------------
 
bool CHDLC_Channel::CTransport::transmitBuffer(HDLC_Buffer_t Buffer) {
//send the start transmission flag 
    ChannelSettings_->thisChannel->TransmitterOctet(HDLC_FLG);
    
    for(const auto i: Buffer){
        switch(i){
            case HDLC_FLG:
                ChannelSettings_->thisChannel->TransmitterOctet(B_STUF_ESC_SEQ);
                ChannelSettings_->thisChannel->TransmitterOctet(B_STUF_FLG);
                break;
            case B_STUF_ESC_SEQ:
                ChannelSettings_->thisChannel->TransmitterOctet(B_STUF_ESC_SEQ);
                ChannelSettings_->thisChannel->TransmitterOctet(B_STUF_ESC);
                break;
            default:
                ChannelSettings_->thisChannel->TransmitterOctet(i);
                break;
            }      
    }
//send the end transmission flag 
    ChannelSettings_->thisChannel->TransmitterOctet(HDLC_FLG);
    return true;
}

//------------------------------------------------------------------------------

void CHDLC_Channel::CTransport::receiveOctet(const uint8_t Octet) {
           
    if((Octet == HDLC_FLG) && !Buffer_.empty()){
        CTransport::ChannelSettings_->Frame->receiveBuffer(Buffer_);
        Buffer_.clear();
    }
    
    switch(Octet){
        case HDLC_FLG:
            break;
            
        case B_STUF_ESC_SEQ:
            ByteStuffingFlag_ = true;
            break;
            
        case B_STUF_FLG:
            if(ByteStuffingFlag_) {
                Buffer_.push_back(HDLC_FLG);
                ByteStuffingFlag_ = false;
            } else Buffer_.push_back(Octet);
            break;
            
        case B_STUF_ESC:
            if (ByteStuffingFlag_) {
                Buffer_.push_back(B_STUF_ESC_SEQ);
                ByteStuffingFlag_ = false;
            } else Buffer_.push_back(Octet);
            break;
            
        default:
            Buffer_.push_back(Octet);
            break;          
    }
    
}

//------------------------------------------------------------------------------

void CHDLC_Channel::CTransport::getStatus() const {
}

//------------------------------------------------------------------------------