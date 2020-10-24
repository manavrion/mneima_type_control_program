#include "stdafx.h"
#include "MetaFrame.h"
#include "System.h"



namespace MetaFrame {

    SystemImpl System;

    SystemImpl::SystemImpl() : timeOfBeginingPaint(0), graphics(null){

    }




    SystemImpl::~SystemImpl() {
    }
}
