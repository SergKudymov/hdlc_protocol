/* 
 * File:   NormalResponseState.h
 * Author: serg
 *
 * Created on April 13, 2017, 11:32 PM
 */

#ifndef NORMALRESPONSESTATE_H
#define NORMALRESPONSESTATE_H

#include "CHDLC_Channel.h"
#include "IState.h"
#include <array>
#include "CFrame.h"
#include <memory>


class CHDLC_Channel::CStateNR: public CHDLC_Channel::IState {
public:
    static const uint32_t FRAMES_BUFFER_SIZE = 8;
    
    static CStateNR* getInstance();
    void initialize(CHDLC_Channel::SChannelSettings* ChannelSettings);
                           
    ~CStateNR();
    
    virtual bool receiveFrame(Frame_t Frame) override;
    virtual bool transmitBuffer(Channel_t Channel, HDLC_Buffer_t Buffer) override;
    void setMode(EFormat_U Mode);
    uint8_t getReceiveNumber() const;
    
private:
    class CTransmitException{
    public:
        CTransmitException(std::string Description){
            std::cout<<"CTransmitException: "<<Description<<std::endl;
        }
    };

    typedef std::vector<CHDLC_Channel::CFrame> FramesBuffer_t;
    static const uint32_t       NUMBER_CHECKPOINT_ = 1;
    bool                        FlagStartTrans_;
    bool                        FlagStartRec_;
    FramesBuffer_t              BufferSend_;
    FramesBuffer_t              BufferReceive_;
    uint32_t                    NumberSend_;
    uint32_t                    NumberReceive_;
    
    CStateNR();
    CStateNR(const CStateNR& orig);
    
    CHDLC_Channel::SChannelSettings* ChannelSettings_;
    void AddFrameReceive(std::shared_ptr<CHDLC_Channel::CFrame>& Frame);
    void AddFrameTransmit(std::shared_ptr<CHDLC_Channel::CFrame>& Frame);
    bool isNextFrameRead() const;
    void UserQueFill();
            
};

#endif /* NORMALRESPONSESTATE_H */

