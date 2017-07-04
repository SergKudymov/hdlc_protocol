/* 
 * File:   NormalResponseState.cpp
 * Author: serg
 * 
 * Created on April 13, 2017, 11:32 PM
 */

#include <memory>

#include "CStateNR.h"

//------------------------------------------------------------------------------

CHDLC_Channel::CStateNR::~CStateNR() {
}

//------------------------------------------------------------------------------

CHDLC_Channel::CStateNR::CStateNR():
        FlagStartTrans_             (true),
        FlagStartRec_               (true),
        BufferSend_                 (FRAMES_BUFFER_SIZE, CFrame()),
        BufferReceive_              (FRAMES_BUFFER_SIZE, CFrame()),
        NumberSend_                 (0),
        NumberReceive_              (0),
        ChannelSettings_            (nullptr)
{    
    BufferReceive_.reserve(FRAMES_BUFFER_SIZE);
}

//------------------------------------------------------------------------------

CHDLC_Channel::CStateNR* CHDLC_Channel::CStateNR::getInstance() {
        static CStateNR instance;
        return &instance;
}

//------------------------------------------------------------------------------

void CHDLC_Channel::CStateNR::initialize(CHDLC_Channel::SChannelSettings* ChannelSettings)
{
    CStateNR::ChannelSettings_ = ChannelSettings;
}

//------------------------------------------------------------------------------

bool CHDLC_Channel::CStateNR::receiveFrame(Frame_t Frame)
{
    auto frame_local = std::make_shared<CHDLC_Channel::CFrame>(*Frame);
    const uint8_t numbReceive = frame_local->getNumberReceive();
    
    CHDLC_Channel::HDLC_Buffer_t buffer;
    
    bool flagReceive = false;
    
    switch(frame_local->getFormat()){
        case FORMAT_S:
            if(frame_local->getFormat_S() == S_FORMAT_RR){
                if((NumberSend_+1) == static_cast<uint32_t>(numbReceive)){
                    for(uint32_t i = 0; i < BufferSend_.size(); ++i) {
                        if(BufferSend_[i].getStatus() == FRAME_STATUS_UNDEFINED)
                            continue;
                        else BufferSend_[i].setStatus(FRAME_STATUS_SEND);
                    }
                } 
                NumberSend_ = numbReceive-1;
            }
            break;
            
        case FORMAT_U:
            CStateNR::ChannelSettings_->thisChannel->setMode(frame_local->getFormat_U());
            return CStateNR::ChannelSettings_->Frame->transmitBuffer(S_FORMAT_RR, CHECKPOINT_UNDEFINED, 0, buffer);
            break;
   
        case FORMAT_I:
            if(numbReceive == NumberReceive_){
                flagReceive = true;
                AddFrameReceive(frame_local);
            }
            
            if(frame_local->getCheckPoint() == ECheckPoint::CHECKPOINT_P){
                if(flagReceive) { CStateNR::UserQueFill();}
                return CStateNR::ChannelSettings_->Frame->transmitBuffer(S_FORMAT_RR,
                                                                         CHECKPOINT_P,
                                                                         NumberReceive_+1,
                                                                         buffer);
            }
            break;
            
    }
}

//------------------------------------------------------------------------------

bool CHDLC_Channel::CStateNR::transmitBuffer(Channel_t Channel, HDLC_Buffer_t Buffer)
{
    auto frame_local = std::make_shared<CHDLC_Channel::CFrame>(CFrame());
    ECheckPoint checkPoint = CHECKPOINT_UNDEFINED;
    
    if(!isNextFrameRead())
        throw CTransmitException("Cannot send a frame, previous frames doesn't сonfirmed");
    
    if ((NumberSend_+1 == NUMBER_CHECKPOINT_) && !FlagStartTrans_)
        checkPoint = CHECKPOINT_P;
    else checkPoint = CHECKPOINT_UNDEFINED;

    frame_local->initialize(NumberSend_, checkPoint, NumberReceive_, Buffer, FRAME_STATUS_WAIT);
    AddFrameTransmit(frame_local);

    if(ChannelSettings_->Frame->transmitBuffer(checkPoint, NumberSend_, NumberReceive_, Buffer)){
        return true;            
    } else {
        if(NumberSend_ != 0)
            --NumberSend_;
        else if(!FlagStartTrans_){
            NumberSend_ = FRAMES_BUFFER_SIZE - 1;
        }
        return false;
    }
        
}

//------------------------------------------------------------------------------

void CHDLC_Channel::CStateNR::AddFrameTransmit(std::shared_ptr<CHDLC_Channel::CFrame> &Frame)
{
    if(FlagStartTrans_){
        BufferSend_.at(0) = *Frame;
        FlagStartTrans_ = false;
        return;
    }
    uint32_t posNext = 0;
    
    if(NumberSend_ != FRAMES_BUFFER_SIZE-1)
        posNext = ++NumberSend_;
    
    BufferSend_.at(posNext) = *Frame;
}

//------------------------------------------------------------------------------

void CHDLC_Channel::CStateNR::AddFrameReceive(std::shared_ptr<CHDLC_Channel::CFrame> &Frame)
{
    if(FlagStartRec_){
        BufferReceive_.at(0) = *Frame;
        FlagStartRec_ = false;
        return;
    }
    uint32_t posNext = 0;

    if(NumberReceive_ != FRAMES_BUFFER_SIZE-1)
        posNext = ++NumberReceive_;

    BufferReceive_.at(posNext) = *Frame;
}

//------------------------------------------------------------------------------

void CHDLC_Channel::CStateNR::UserQueFill(){
    for(auto i: BufferReceive_) {ChannelSettings_->UserQue.push(i);}
}

//------------------------------------------------------------------------------

uint8_t CHDLC_Channel::CStateNR::getReceiveNumber() const{
    return static_cast<uint8_t> (NumberReceive_);
}

//------------------------------------------------------------------------------

bool CHDLC_Channel::CStateNR::isNextFrameRead() const {
    uint32_t posNext = 0;

    if(NumberSend_ != FRAMES_BUFFER_SIZE-1)
        posNext = NumberSend_+1;
    EFrameStatus frameOnBufferStatus = BufferSend_.at(posNext).getStatus();

    if(frameOnBufferStatus == FRAME_STATUS_WAIT)
        return false;
    else return true;
}

//------------------------------------------------------------------------------