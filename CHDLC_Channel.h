/* 
 * File:   HDLCChannel.h
 * Author: serg
 *
 * Created on April 9, 2017, 1:54 PM
 */

#ifndef CHDLC_CHANNEL_H
#define CHDLC_CHANNEL_H

#include <iostream>
#include <stdint.h>
#include <vector>
#include <queue>
#include <memory.h>


class CHDLC_Channel {

public:
    /* UI (Unnumbered information - Ненумерованная информация) . Эта команда
     * позволяет производить передачу данных пользователя в не нумерованном
     * кадре (т. е. без порядкового номера)
     * 
     * Режим нормального ответа (NRM - Normal Response Mode) требует, чтобы 
     * прежде, чем начать передачу, вторичная станция получила явное разрешение
     *  от первичной. */
    enum EFormat_U: uint8_t {
        U_FORMAT_UNDEFINED =    0x00,
        U_FORMAT_NRM =          0xc1,
        U_FORMAT_UI =           0xc0,
        U_FORMAT_TEST =         0xc7,                 
        U_FORMAT_DISC =         0xc2,
    };
    
    typedef std::vector<uint8_t>    HDLC_Buffer_t;
    typedef uint32_t                Address_t;
//------------------------------------------------------------------------------
    /* Пользователь определяет функцию для приёма/обработки данных*/
    typedef void (* DataReceiver_t)(HDLC_Buffer_t Buffer);
    typedef void (* DataTransmitter_t)(const uint8_t Octet);
    DataReceiver_t ReceiverBufferHandler;
    DataTransmitter_t TransmitterOctet;
//------------------------------------------------------------------------------

    
    CHDLC_Channel(Address_t AddressSelf, EFormat_U Mode);
    ~CHDLC_Channel();
    
    bool transmitBuffer(Address_t AddressTarget, HDLC_Buffer_t Buffer);
    void receiveOctet(const uint8_t Octet);
    
    HDLC_Buffer_t getData() const;
    
    EFormat_U getMode() const;
    void setMode(EFormat_U Mode);
    
    void setAddressSelf(Address_t AdderessSelf);
    void setAddressTarget(Address_t AddressTarget); 
    Address_t getAddressSelf() const;
    Address_t getAddressTarget() const;

private:

    class IState;

    class CStateInitialize;
    class CStateDisconnect;
    class CStateUI;
    class CStateNR;
    class CNotSupportedException;
    
    class CFrame;
    class CTransport;
   
    typedef CTransport*                            Transporter_t;
    typedef CFrame*                                Frame_t;
    typedef CHDLC_Channel*                         Channel_t;
    
    struct SChannelSettings {
        Address_t                               AddressSelf;
        Address_t                               AddressTarget;
        EFormat_U                               Mode;
        std::queue<CHDLC_Channel::CFrame>       UserQue;
        
        Channel_t                               thisChannel;
        Frame_t                                 Frame;
        Transporter_t                           Transport;
        
        SChannelSettings(Channel_t Channel, Address_t AddressSelf);
        ~SChannelSettings();
        
    };
    
    enum EFormat: uint8_t {
        FORMAT_UNDEFINED =      0x00,
        /* Информационный формат (I - формат) используется для передачи данных
         * конечных пользователей между двумя станциями*/
        FORMAT_I =              0x01,
        
        /* Супервизорный формат (S - формат) выполняет управляющие функции: 
         * подтверждение (квитирование) кадров, запрос на повторную передачу кадров и 
         * запрос на временную задержку передачи кадров.*/    
        FORMAT_S =              0x02,
        
        /* Ненумерованный формат (U - формат) для целей управления: установка режима,
         *  тестирования, сброса и идентификации станции и т.д.*/
        FORMAT_U =              0x03
    };
    
    enum EFormat_S: uint8_t {
        S_FORMAT_UNDEFINED =    0x00,
        S_FORMAT_RR =           0x80,      /* Код готовности к приёму(Receive Ready)*/
        S_FORMAT_RNR =          0x0a      /* Код не готовности к приёму(Receive Not Ready)*/
    };//sInstructionR_, sInstructionT_;
    
    /* P - when Primary station, F - Following station*/
    enum ECheckPoint: uint8_t {
        CHECKPOINT_UNDEFINED =  0x00,
        CHECKPOINT_P =          0x01,
        CHECKPOINT_F =          0x01
    };
    
    enum EFrameStatus {
        FRAME_STATUS_UNDEFINED,
        FRAME_STATUS_READY,
        FRAME_STATUS_READ,
        FRAME_STATUS_WAIT,
        FRAME_STATUS_SEND
    };
    
    IState* State_;
    SChannelSettings* ChannelSettings_;
    
    void changeState(CHDLC_Channel::IState* newState);
    
    void receiveFrame(Frame_t Frame);
};

#endif /* CHDLC_CHANNEL_H */

