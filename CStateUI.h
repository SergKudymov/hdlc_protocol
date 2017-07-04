/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UnnumberedInformationState.h
 * Author: serg
 *
 * Created on April 13, 2017, 11:33 PM
 */

#ifndef CUNNUMBEREDINFORMATIONSTATE_H
#define CUNNUMBEREDINFORMATIONSTATE_H
#include "CHDLC_Channel.h"
#include "IState.h"

class CHDLC_Channel::CStateUI: public CHDLC_Channel::IState {
public:
    
    static CStateUI* getInstance();
    ~CStateUI();
    
private:
    
    CStateUI();
    CStateUI(const CStateUI& orig);
};

#endif /* UNNUMBEREDINFORMATIONSTATE_H */

