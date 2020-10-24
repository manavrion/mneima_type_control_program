#include "stdafx.h"
namespace MetaFrame {

    

    

    




    Color::Color() : red(0), green(0), blue(0), alpha(255) {}

    Color::Color(byte red, byte green, byte blue) : red(red), green(green), blue(blue), alpha(255){}

    Color::Color(byte red, byte green, byte blue, byte alpha) : red(red), green(green), blue(blue), alpha(alpha) {}

    Color::Color(const Color &color) : red(color.red), green(color.green), blue(color.blue), alpha(color.alpha) {}

    byte Color::getAlpha() const {
        return alpha;
    }

    byte Color::getRed() const {
        return red;
    }

    byte Color::getGreen() const {
        return green;
    }

    byte Color::getBlue() const {
        return blue;
    }

    Color::operator const Gdiplus::Color() const {
        return Gdiplus::Color(alpha, red, green, blue);
    }
}