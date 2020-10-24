#pragma once
#include "Point.h"
#include "Size.h"



namespace MetaFrame {
    class Rect {
    public:
        Rect();
        Rect(int x, int y, int width, int height);
        Rect(const Point& location, const Size& size);
        explicit Rect(const Size& size);

        Rect(const Rect &a, const Rect &b);

        //platform
        Rect(RECT lprect);

        int x;
        int y;
        int width;
        int height;

        Point getLocation() const;
        Size getSize() const;

        Rect getBounds() const;
        int getLeftPos() const;
        int getTopPos() const;
        int getRightPos() const;
        int getBottomPos() const;


        void SetSize(const Size &size);
        void setPoint(const Point &point);

        bool isEmptyArea() const;

        virtual bool contains(int x, int y) const;
        virtual bool contains(const Point& pt) const;
        virtual bool contains(const PointF& pt) const;
        virtual bool contains(Rect& rect) const;
        virtual void inflate(int dx, int dy);
        virtual void inflate(const Point& point);

        virtual bool intersect(const Rect& rect) const;

        virtual void offset(const Point& point);
        virtual void offset(int dx, int dy);

        virtual void shiftTo(Point point);


        //platform
        operator const Gdiplus::Rect() const;
        explicit operator const Gdiplus::RectF() const;
        operator const RECT() const;
    };

}