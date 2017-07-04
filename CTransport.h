/* 
 * File:   HDLC_transport.h
 * Author: serg
 *
 * Created on April 9, 2017, 8:02 PM
 */

#ifndef HDLC_TRANSPORT_H
#define HDLC_TRANSPORT_H

#include "stdint.h"
#include "CHDLC_Channel.h"


class CHDLC_Channel::CTransport {
public:
    CTransport(SChannelSettings* ChannelSettings);
    
    ~CTransport();

    bool transmitBuffer(HDLC_Buffer_t Buffer);
    
    void receiveOctet(const uint8_t Octet);
    
    void getStatus() const;
    
private:    
    CTransport(const CTransport& orig);
    
    static const uint8_t HDLC_FLG;

    static const uint8_t B_STUF_ESC_SEQ;
    static const uint8_t B_STUF_FLG;
    static const uint8_t B_STUF_ESC;
    
    
    CHDLC_Channel::SChannelSettings* ChannelSettings_;
        
    bool ByteStuffingFlag_;
    
    HDLC_Buffer_t Buffer_;
};

#endif /* HDLC_TRANSPORT_H */

