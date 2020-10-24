#pragma once

namespace MetaFrame {
    
    struct Point {
    public:
        inline Point() : x(0), y(0) {}
        inline Point(const Point &point) : x(point.x), y(point.y) {}
        inline Point(int x, int y) : x(x), y(y) {}

        int x;
        int y;

        inline Point &operator+=(const Point& point) {
            x += point.x;
            y += point.y;
            return *this;
        };
        inline Point &operator-=(const Point& point) {
            x -= point.x;
            y -= point.y;
            return *this;
        }

        inline Point operator-() const {
            return Point(-x, -y);
        };
        inline Point operator+() const {
            return Point(x, y);
        };

        //platform
        inline operator const Gdiplus::Point() const {
            return Gdiplus::Point(x, y);
        };
    };

    inline Point operator+(const Point &a, const Point &b) {
        return Point(a.x + b.x, a.y + b.y);
    };
    inline Point operator-(const Point &a, const Point &b) {
        return Point(a.x - b.x, a.y - b.y);
    };
    inline bool operator==(const Point &a, const Point &b) {
        return (a.x == b.x) && (a.y == b.y);
    };
    inline bool operator!=(const Point &a, const Point &b) {
        return !(a == b);
    };


    class PointF {
    public:
        inline PointF() : x(), y() {}
        inline PointF(const PointF &pointF) : x(pointF.x), y(pointF.y) {}
        inline PointF(const Point &point) : x(point.x), y(point.y) {}
        inline PointF(float x, float y) : x(x), y(y) {}

        float x;
        float y;

        inline PointF &operator+=(const PointF& point) {
            x += point.x;
            y += point.y;
            return *this;
        };
        inline PointF &operator-=(const PointF& point) {
            x -= point.x;
            y -= point.y;
            return *this;
        }

        inline PointF operator-() const {
            return PointF(-x, -y);
        };
        inline PointF operator+() const {
            return PointF(x, y);
        };

        explicit inline operator const Point() const {
            return Point(int(x), int(y));
        }

        //platform
        inline operator const Gdiplus::PointF() const {
            return Gdiplus::PointF(x, y);
        };

    };

    inline PointF operator+(const PointF &a, const PointF &b) {
        return PointF(a.x + b.x, a.y + b.y);
    };
    inline PointF operator-(const PointF &a, const PointF &b) {
        return PointF(a.x - b.x, a.y - b.y);
    };
    inline bool operator==(const PointF &a, const PointF &b) {
        return (abs(a.x - b.x) < 0.00001) && (abs(a.y - b.y) < 0.00001);
    };
    inline bool operator!=(const PointF &a, const PointF &b) {
        return !(a == b);
    };



}