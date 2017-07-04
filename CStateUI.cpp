/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UnnumberedInformationState.cpp
 * Author: serg
 * 
 * Created on April 13, 2017, 11:33 PM
 */

#include "CStateUI.h"

CHDLC_Channel::CStateUI* CHDLC_Channel::CStateUI::getInstance(){
    static CStateUI instance;
    return &instance;
}

CHDLC_Channel::CStateUI::CStateUI() {
}

CHDLC_Channel::CStateUI::~CStateUI() {
}

