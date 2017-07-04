/* 
 * File:   InitializeState.h
 * Author: serg
 *
 * Created on April 13, 2017, 11:26 PM
 */

#ifndef CSTATEINITIALIZE_H
#define CSTATEINITIALIZE_H

#include "IState.h"
#include "CHDLC_Channel.h"

class CHDLC_Channel::CStateInitialize: public CHDLC_Channel::IState {
    public:
        ~CStateInitialize();
        
        static CStateInitialize* getInstance();
        void initialize(CHDLC_Channel::SChannelSettings* ChannelSettings_);
        
        virtual bool receiveFrame(Frame_t Frame) override;
   
    private:
        CStateInitialize();
        CHDLC_Channel::SChannelSettings* ChannelSettings_;
    };

#endif /* CSTATEINITIALIZE_H */

