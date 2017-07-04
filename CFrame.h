/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HDLC_frame.h
 * Author: serg
 *
 * Created on April 9, 2017, 8:02 PM
 */

/*------------------------------------------------------------------------------
 *              Control field 
 * ___________________________________________________
 * |bit       |  1|   2|   3|   4|   5|   6|   7|   8 |
 * |__________|___|____|____|____|____|____|____|_____|
 * |I-format  |  0|    |N(s)     |P/F|| N(R)          |
 * |__________|___|____|_________|____|_______________|
 * |S-format  |  1|   0|Scode    |P/F | N(R)          |                                                 
 * |__________|___|____|_________|____|_______________|
 * |U-format  |  1|   1|Ucode    |P/F | Ucode         |
 * |__________|___|____|_________|____|_______________| 
 *  
 * -----------------------------------------------------------------------------    
 */

#ifndef HDLC_FRAME_H
#define HDLC_FRAME_H

#include <stdint.h>
#include "crc.h"
#include "CHDLC_Channel.h"


class CHDLC_Channel::CFrame {
public:
    CFrame(SChannelSettings* ChannelSettings);
    CFrame();

    ~CFrame();

    bool transmitBuffer(EFormat_U U_Istruction, ECheckPoint CheckPoint, HDLC_Buffer_t Buffer);
    
    bool transmitBuffer(EFormat_S S_Instruction, ECheckPoint CheckPoint, uint8_t NumberReceive, HDLC_Buffer_t Buffer);
    
    bool transmitBuffer(ECheckPoint CheckPoin, uint8_t NumberSend, uint8_t NumberReceive, HDLC_Buffer_t Buffer);
    
    bool receiveBuffer(HDLC_Buffer_t Buffer);
    
    HDLC_Buffer_t getData() const;
    
    EFormat_S getFormat_S() const;
    
    EFormat_U getFormat_U() const;
    
    EFormat getFormat() const;
    
    uint8_t getNumberReceive() const;
    
    uint8_t getNumberSend() const;
    
    ECheckPoint getCheckPoint() const;
    
    EFrameStatus getStatus() const;
    
    void setStatus(EFrameStatus Status);
    
    void initialize(uint8_t NumberFrameSend, ECheckPoint CheckPoint, 
                    uint8_t NumberFrameReceive,HDLC_Buffer_t Buffer, EFrameStatus Status);
    
private:    
    static const uint32_t FRAME_SIZE = 256;
                
    EFormat                 Format_;
    EFormat_U               Instruction_U_;
    EFormat_S               Instruction_S_;
    ECheckPoint             CheckPoint_;
    EFrameStatus            Status_;
    uint8_t                 NumberFrameReceive_;
    uint8_t                 NumberFrameSend_;
    std::vector<uint8_t>    Buffer_;
    
    CHDLC_Channel::SChannelSettings* ChannelSettings_;
    
    void setAddress(HDLC_Buffer_t& Buffer);
    
    Address_t getAddress(const HDLC_Buffer_t& Buffer)const;
   
    void setCRC(HDLC_Buffer_t& Buffer);
    
    crc_t getCRC(const HDLC_Buffer_t& Buffer) const;
    
    bool checkCRC(HDLC_Buffer_t& Buffer)const;
    
};

#endif /* HDLC_FRAME_H */

