/*
 * File:   HDLCChannel.cpp
 * Author: serg
 * 
 * Created on April 9, 2017, 1:54 PM
 */

#include "CHDLC_Channel.h"
#include "IState.h"
#include "CStateInitialize.h"
#include "CStateNR.h"
#include "CNotSupportedException.h"
#include "CTransport.h"

//------------------------------------------------------------------------------
        
CHDLC_Channel::SChannelSettings::SChannelSettings(Channel_t Channel, Address_t AddressSelf):
    AddressSelf             (AddressSelf),
    AddressTarget           (0),
    Mode                    (U_FORMAT_DISC),
    UserQue                 (),
    thisChannel             (Channel),
    Frame                   (new CHDLC_Channel::CFrame(this)),
    Transport               (new CHDLC_Channel::CTransport(this))
{}

//------------------------------------------------------------------------------

CHDLC_Channel::SChannelSettings::~SChannelSettings(){
    delete Frame;
    delete Transport;
}

//------------------------------------------------------------------------------

CHDLC_Channel::CHDLC_Channel(Address_t AddressSelf, EFormat_U Mode):
        State_                  (CHDLC_Channel::CStateInitialize::getInstance()),
        ChannelSettings_        (new SChannelSettings(this, AddressSelf))
{
    setMode(Mode);
    CStateInitialize::getInstance()->initialize(ChannelSettings_);
    CStateNR::getInstance()->initialize(ChannelSettings_);
}

//------------------------------------------------------------------------------

CHDLC_Channel::~CHDLC_Channel(){
    delete ChannelSettings_;
}

//------------------------------------------------------------------------------

void CHDLC_Channel::changeState(IState* newState){
    State_ = newState;
}

//------------------------------------------------------------------------------

bool CHDLC_Channel::transmitBuffer(Address_t TargetAddress, HDLC_Buffer_t Buffer){
    ChannelSettings_->AddressTarget = TargetAddress;
    State_->transmitBuffer(this, Buffer);
}

//------------------------------------------------------------------------------

void CHDLC_Channel::receiveOctet(const uint8_t Octet){
    ChannelSettings_->Transport->receiveOctet(Octet);
}

//------------------------------------------------------------------------------

void CHDLC_Channel::receiveFrame(Frame_t Frame){
    State_->receiveFrame(Frame);
}

//------------------------------------------------------------------------------

CHDLC_Channel::EFormat_U CHDLC_Channel::getMode() const {
    return ChannelSettings_->Mode;
}

//------------------------------------------------------------------------------

void CHDLC_Channel::setMode(EFormat_U Mode) {
    ChannelSettings_->Mode = Mode;
    switch(Mode) {
        case U_FORMAT_NRM:
            this->changeState(CHDLC_Channel::CStateNR::getInstance());
            break;

        case U_FORMAT_DISC:
            throw CNotSupportedException("CHDLC_Channel::setMode case U_FORMAT_DISC");

        case U_FORMAT_UI:
            //this->changeState(HDLC_Channel::UnnumberedInformationState::getInstance());
            break;

        case U_FORMAT_TEST:
            throw CNotSupportedException("CHDLC_Channel::setMode case U_FORMAT_TEST");
    }
}

//------------------------------------------------------------------------------

CHDLC_Channel::HDLC_Buffer_t CHDLC_Channel::getData() const{
    HDLC_Buffer_t buffer = ChannelSettings_->UserQue.front().getData();
    ChannelSettings_->UserQue.pop();
    return buffer;
}

//------------------------------------------------------------------------------

CHDLC_Channel::Address_t CHDLC_Channel::getAddressSelf() const{
    return ChannelSettings_->AddressSelf;
}

//------------------------------------------------------------------------------

CHDLC_Channel::Address_t CHDLC_Channel::getAddressTarget() const{
    return ChannelSettings_->AddressTarget;
}

//------------------------------------------------------------------------------

void CHDLC_Channel::setAddressTarget(Address_t AddressTarget){
    ChannelSettings_->AddressTarget = AddressTarget;
}


