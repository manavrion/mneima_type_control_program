#pragma once
#include "stdafx.h"

#include "String.h"

#include "Point.h"
#include "Size.h"

#include "Rect.h"
namespace MetaFrame {

    //typedef Gdiplus::Color Color;//todo rewrite


    template<typename T1, typename T2>
    using HashMap = std::unordered_map<T1, T2>;

    template<typename T>
    using ArrayList = std::vector<T>;




    class Color {
    public:
        Color();
        Color(byte red, byte green, byte blue);
        Color(byte red, byte green, byte blue, byte alpha);
        Color(const Color &color);

        byte red;
        byte green;
        byte blue;
        byte alpha;

        byte getAlpha() const;
        byte getRed() const;
        byte getGreen() const;
        byte getBlue() const;

        bool operator==(const Color &color) const {
            return (red == color.red) && (green == color.green) && (blue == color.blue) && (alpha == color.alpha);
        }

        //platform
        operator const Gdiplus::Color() const;

    };

    struct Margin {
        int left;
        int right;
        int top;
        int bottom;
        Margin( int left,
                int right,
                int top,
                int bottom) :
            left(left),
            right(right),
            top(top),
            bottom(bottom) {
        };
        Margin():
            left(0),
            right(0),
            top(0),
            bottom(0) {};
    };


    enum class HorizontalAlignment {
        Absolute,
        Left,
        Center,
        Right,
        Stretch
    };

    enum class VerticalAlignment {
        Absolute,
        Top,
        Center,
        Bottom,
        Stretch
    };
}