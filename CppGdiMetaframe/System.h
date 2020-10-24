#pragma once
#include "Graphics.h"
namespace MetaFrame {

    class SystemImpl {
    public:
        SystemImpl();

        int timeOfBeginingPaint;

        Gdiplus::Graphics *graphics;

        ~SystemImpl();
    };

    extern SystemImpl System;

}


