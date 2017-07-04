/* 
 * File:   HDLC_frame.cpp
 * Author: serg
 * 
 * Created on April 9, 2017, 8:02 PM
 */

#include <vector>
#include <algorithm>

#include "CFrame.h"
#include "CTransport.h"

//------------------------------------------------------------------------------

CHDLC_Channel::CFrame::CFrame(SChannelSettings* ChannelSettings):
                    Format_                 (FORMAT_UNDEFINED),
                    Instruction_U_          (U_FORMAT_UNDEFINED),
                    Instruction_S_          (S_FORMAT_UNDEFINED),
                    CheckPoint_             (CHECKPOINT_UNDEFINED),
                    Status_                 (FRAME_STATUS_UNDEFINED),
                    NumberFrameReceive_     (0),
                    NumberFrameSend_        (0),
                    ChannelSettings_        (ChannelSettings)
{
    Buffer_.reserve(FRAME_SIZE);
}

//------------------------------------------------------------------------------

CHDLC_Channel::CFrame::CFrame():
                    Format_                 (FORMAT_UNDEFINED),
                    Instruction_U_          (U_FORMAT_UNDEFINED),
                    Instruction_S_          (S_FORMAT_UNDEFINED),
                    CheckPoint_             (CHECKPOINT_UNDEFINED),
                    Status_                 (FRAME_STATUS_UNDEFINED),
                    NumberFrameReceive_     (0),
                    NumberFrameSend_        (0),
                    ChannelSettings_        (nullptr)
{
    Buffer_.reserve(FRAME_SIZE);
}

//------------------------------------------------------------------------------

CHDLC_Channel::CFrame::~CFrame() {}

//------------------------------------------------------------------------------

bool CHDLC_Channel::CFrame::receiveBuffer(HDLC_Buffer_t Buffer){
    
    if(!checkCRC(Buffer))
        return false;
    
    Address_t idReceive = getAddress(Buffer);
    if(idReceive != CFrame::ChannelSettings_->AddressSelf)
        return false;
    Buffer.erase(Buffer.begin(), Buffer.begin()+sizeof(Address_t));
   
    CheckPoint_ = static_cast<ECheckPoint>((Buffer.at(0) >> 3) & (~0xfe));
    
    switch(Buffer.at(0) >> 6){
        case FORMAT_S:
            Format_ =               FORMAT_S;
            Instruction_U_ =        U_FORMAT_UNDEFINED;
            Instruction_S_ =        static_cast<EFormat_S>(Buffer[0] & (~0x4f));
            NumberFrameReceive_ =   (Buffer[0] & (~0xf8));
            NumberFrameSend_ =      0;
            break;
            
        case FORMAT_U:
            Format_ =               FORMAT_U;
            Instruction_U_ =        static_cast<EFormat_U>(Buffer[0] & (~0xc8));
            Instruction_S_ =        S_FORMAT_UNDEFINED;
            NumberFrameReceive_ =   0;
            NumberFrameSend_ =      0;
            break;
            
        case 0x00:
        case FORMAT_I:
            Format_ =               FORMAT_I;
            Instruction_U_ =        U_FORMAT_UNDEFINED;
            Instruction_S_ =        S_FORMAT_UNDEFINED;
            NumberFrameReceive_ =   (Buffer[0] & (~0xf8));
            NumberFrameSend_ =      (Buffer[0] >> 4);
            break;            
    }
    Buffer.erase(Buffer.begin(), Buffer.begin()+1);
    Buffer_ = Buffer;
    CFrame::ChannelSettings_->thisChannel->receiveFrame(this);
}

//------------------------------------------------------------------------------
// U - frame 
bool CHDLC_Channel::CFrame::transmitBuffer(EFormat_U U_Instruction, ECheckPoint CheckPoint,
                                        HDLC_Buffer_t Buffer)
{
    uint8_t control = static_cast<uint8_t>(U_Instruction);
    control |= (static_cast<uint8_t> (CheckPoint) << 3);
    
    Buffer.insert(Buffer.begin(), control);
    setAddress(Buffer);
    setCRC(Buffer);
    
    if(CFrame::ChannelSettings_->Transport->transmitBuffer(Buffer))
        return true;
    else return false;
}

//------------------------------------------------------------------------------
// S - frame 
bool CHDLC_Channel::CFrame::transmitBuffer(EFormat_S S_Instruction, ECheckPoint CheckPoint,
                                        uint8_t NumberReceive, HDLC_Buffer_t Buffer)
{   
    uint8_t control = static_cast<uint8_t> (S_Instruction);
    
    control |= static_cast<uint8_t> (CheckPoint) << 3;
    control |= NumberReceive;
    
    Buffer.insert(Buffer.begin(), control);
    setAddress(Buffer);
    setCRC(Buffer);
    
    if(CHDLC_Channel::CFrame::ChannelSettings_->Transport->transmitBuffer(Buffer))
        return true;
    else return false;
}

//------------------------------------------------------------------------------
// I - frame 
bool CHDLC_Channel::CFrame::transmitBuffer(ECheckPoint CheckPoint, uint8_t NumberSend,
                                                uint8_t NumberReceive, HDLC_Buffer_t Buffer)
{
    uint8_t control = 0x00;
    control |= NumberSend << 4;
    control |= static_cast<uint8_t> (CheckPoint) << 3;
    control |= NumberReceive;
    
    Buffer.insert(Buffer.begin(), control);
    setAddress(Buffer);
    setCRC(Buffer);
    
    if(CHDLC_Channel::CFrame::ChannelSettings_->Transport->transmitBuffer(Buffer))
        return true;
    else return false;
}

//------------------------------------------------------------------------------

void CHDLC_Channel::CFrame::setAddress(HDLC_Buffer_t& Buffer){
    Address_t adr = CHDLC_Channel::CFrame::ChannelSettings_->AddressTarget;
    
    uint8_t* adrTemp = reinterpret_cast<uint8_t*> (&adr);
    std::reverse(adrTemp, adrTemp + sizeof(Address_t));
    Buffer.insert(Buffer.begin(), adrTemp, adrTemp + sizeof(Address_t));
}

//------------------------------------------------------------------------------

CHDLC_Channel::Address_t CHDLC_Channel::CFrame::getAddress(const HDLC_Buffer_t& Buffer) const{
    Address_t address = 0x00;
    for(uint32_t i = 0; i < sizeof(Address_t); ++i){
        address <<= 8;
        address |= Buffer[i];
    }
    return address;
}

//------------------------------------------------------------------------------

void CHDLC_Channel::CFrame::setCRC(HDLC_Buffer_t& Buffer){
    crc_t crcFrame = 0x00;

    crcFrame = crc_normal(&Buffer[0], static_cast<uint32_t>(Buffer.size()));
    uint8_t* crcTemp = reinterpret_cast<uint8_t*>(&crcFrame);
    std::reverse(crcTemp, crcTemp + sizeof(crc_t));
    Buffer.insert(Buffer.end(), crcTemp, crcTemp + sizeof(crc_t));
}

//------------------------------------------------------------------------------

crc_t CHDLC_Channel::CFrame::getCRC(const HDLC_Buffer_t& Buffer) const{
    crc_t crcFrame = 0x00;
    std::vector<uint8_t> crcVec;
    crcVec.insert(crcVec.begin(), Buffer.end()-sizeof(crc_t), Buffer.end());
    
     for(const auto i: crcVec) {
        crcFrame <<= 8;
        crcFrame |= i;
    }
    return crcFrame;
}

//------------------------------------------------------------------------------

bool CHDLC_Channel::CFrame::checkCRC(HDLC_Buffer_t& Buffer) const{
    crc_t crcCompute = 0x00;
    crc_t crcFrame = 0x00;
    uint32_t srcSize = sizeof(crc_t);

    crcFrame = getCRC(Buffer);
    
    while(srcSize--) Buffer.pop_back();
    
    crcCompute = crc_normal(&Buffer[0], Buffer.size());
    
    if (crcFrame == crcCompute)
        return true;
    else return false;
}

//------------------------------------------------------------------------------

CHDLC_Channel::EFormat_S CHDLC_Channel::CFrame::getFormat_S() const{
    return Instruction_S_;
}

CHDLC_Channel::EFormat_U CHDLC_Channel::CFrame::getFormat_U() const{
    return Instruction_U_;
}

//------------------------------------------------------------------------------

CHDLC_Channel::EFormat CHDLC_Channel::CFrame::getFormat() const{
    return Format_;
}

//------------------------------------------------------------------------------

uint8_t CHDLC_Channel::CFrame::getNumberReceive() const{
    return NumberFrameReceive_;
}

//------------------------------------------------------------------------------
    
uint8_t CHDLC_Channel::CFrame::getNumberSend() const{
    return NumberFrameSend_;
}

//------------------------------------------------------------------------------

CHDLC_Channel::ECheckPoint CHDLC_Channel::CFrame::getCheckPoint() const{
    return CheckPoint_;
}

//------------------------------------------------------------------------------

CHDLC_Channel::EFrameStatus CHDLC_Channel::CFrame::getStatus() const{
    return Status_;
}

//------------------------------------------------------------------------------

CHDLC_Channel::HDLC_Buffer_t CHDLC_Channel::CFrame::getData() const{
    return Buffer_;
}

//------------------------------------------------------------------------------

void CHDLC_Channel::CFrame::setStatus(EFrameStatus Status){
    Status_ = Status;
}

//------------------------------------------------------------------------------
// init I - format
void CHDLC_Channel::CFrame::initialize(uint8_t NumberFrameSend, ECheckPoint CheckPoint,
                                            uint8_t NumberFrameReceive, HDLC_Buffer_t Buffer, EFrameStatus Status)
{
    Format_ =                   FORMAT_I;
    NumberFrameSend_ =          NumberFrameSend;
    NumberFrameReceive_ =       NumberFrameReceive;
    CheckPoint_ =               CheckPoint;
    Status_ =                   Status;
    Buffer_ =                   Buffer;    
}

//------------------------------------------------------------------------------