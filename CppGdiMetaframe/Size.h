#pragma once

namespace MetaFrame {

    class Size {
    public:

        inline Size() : width(0), height(0) {}
        inline Size(const Size& size) : width(size.width), height(size.height) {}
        inline explicit Size(const Point& point) : width(point.x), height(point.y) {}
        inline Size(int width, int height) : width(width), height(height) {}

        int width;
        int height;

        inline Size operator+(const Size &sz) const {
            return Size(width + sz.width, height + sz.height);
        };
        inline Size operator-(const Size &sz) const {
            return Size(width - sz.width, height - sz.height);
        };
        inline bool operator==(const Size &sz) const {
            return (width == sz.width) && (height == sz.height);
        };
        inline bool empty() const {
            return (width == 0 && height == 0);
        };

        //platform
        inline operator const Gdiplus::Size() const {
            return Gdiplus::Size(width, height);
        }
        inline operator const Gdiplus::Rect() const {
            return Gdiplus::Rect(0, 0, width, height);
        };
    };


}