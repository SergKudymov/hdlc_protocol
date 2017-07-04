cmake_minimum_required(VERSION 3.7)
project(HDLC_ISO)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        CHDLC_Channel.cpp
        CHDLC_Channel.h
        CFrame.cpp
        CFrame.h
        CTransport.cpp
        CTransport.h
        CNotSupportedException.cpp
        CNotSupportedException.h
        crc.cpp
        crc.h
        CStateInitialize.cpp
        CStateInitialize.h
        CStateNR.cpp
        CStateNR.h
        CStateUI.cpp
        CStateUI.h
        CStateDisconnect.cpp
        CStateDisconnect.h
        IState.cpp
        IState.h
        main.cpp)

add_executable(HDLC_ISO ${SOURCE_FILES})