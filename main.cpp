/*
 * File:   main.cpp
 * Author: serg
 *
 * Created on April 9, 2017, 1:50 PM
 */

#include <cstdlib>
#include <memory>
#include "CHDLC_Channel.h"
//#include "crc.h"


//------------------------------------------------------------------------------

CHDLC_Channel::Address_t adr = 0x01;
CHDLC_Channel::EFormat_U mode = CHDLC_Channel::U_FORMAT_NRM;
CHDLC_Channel channel(adr, mode);
CHDLC_Channel::HDLC_Buffer_t BufResponse = {0x7e, 0, 0, 0, 1, 0x8a, 0xf2, 0x3d, 0x7e};
uint32_t counter_FLG = 0;
uint32_t counter_Octet = 0;

//------------------------------------------------------------------------------
// Интерфейс нижнего уровня приёма и передачи для каждого канала
void  DataReceiver(CHDLC_Channel::HDLC_Buffer_t Buffer){
    for(auto i: Buffer) std::cout<<"octet receive: "<<static_cast<uint32_t>(i)<<std::endl;
}
void DataTransmitter (const uint8_t Octet){
    if(counter_Octet == 0) {
        std::cout << "Transmit -----> [";
    }

    std::cout<<std::hex<<static_cast<uint32_t>(Octet)<<' ';
    ++counter_Octet;
    
    if(Octet == 0x7e)
        ++counter_FLG;
    if(counter_Octet == 19)                     // for example, frame have 19 elements
    {
        std::cout<<']'<<std::endl;
        counter_Octet = 0;
    }
}
void Response(CHDLC_Channel::HDLC_Buffer_t Buf){
    std::cout<<"Receive  <----- [";
    for(auto i: BufResponse)
    {
        channel.receiveOctet(i);
        std::cout<<std::hex<<static_cast<uint32_t>(i)<<' ';
    }
    std::cout<<']'<<std::endl;
}

int main(int argc, char** argv) {
    //crc_t crc = crc_normal(&BufResponse[0], BufResponse.size());
    CHDLC_Channel::HDLC_Buffer_t buffer;
    for(uint8_t i = 0x00; i<0x0a; ++i)          //for example, data size 10 elements
    {
        buffer.push_back(i);
    }
 
    channel.ReceiverBufferHandler = &DataReceiver;
    channel.TransmitterOctet = &DataTransmitter;
    
    channel.transmitBuffer(adr, buffer);        // P: информационный кадр, S == 0, R == 0
    channel.transmitBuffer(adr, buffer);        // P: информационный кадр, S == 1, R == 0, опрос

    Response(BufResponse);                      // F: готов к приёму, R == 2, подтверждение
    channel.transmitBuffer(adr, buffer);
    channel.transmitBuffer(adr, buffer);
    channel.transmitBuffer(adr, buffer);
    channel.transmitBuffer(adr, buffer);
    channel.transmitBuffer(adr, buffer);
    channel.transmitBuffer(adr, buffer);

    channel.transmitBuffer(adr, buffer);
    channel.transmitBuffer(adr, buffer);
    channel.transmitBuffer(adr, buffer);
    channel.transmitBuffer(adr, buffer);

    
    //for(auto i: BufferSend) {std::cout<<static_cast<uint32_t>(i)<<std::endl;}

    return 0;

}

