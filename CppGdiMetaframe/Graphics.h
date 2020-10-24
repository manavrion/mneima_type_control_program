#pragma once

#include <windows.h>
#include <ctime>
#include "MetaFrame.h"
#include "MetaTypes.h"

namespace MetaFrame {
#if 0
    class Graphics {
    public:
        Graphics(Size size, Color color, HWND hWindow)
            : hWindow(hWindow), backgroundBrush(color), size(size), totalShifts(), permitedRegion()
        {
            //init backBuffer
            backBuffer = new byte[size.width * size.height * 4];
            backBufferHBitmap = CreateBitmap(size.width, size.height, 1, 32, backBuffer);

            //init HDC and graphics
            backBufferHDC = CreateCompatibleDC(null);
            SelectObject(backBufferHDC, backBufferHBitmap);
            graphics = new Gdiplus::Graphics(backBufferHDC);

            fillBackground(color);


           // startProc();
            //System.graphics = this;
        }

        bool isupdate = false;
        bool isbreak = false;

    private:
        HWND hWindow;
        Gdiplus::SolidBrush backgroundBrush;
        Size size;

        byte *backBuffer; //backBuffer bits
        HBITMAP backBufferHBitmap;

        HDC backBufferHDC;
        Gdiplus::Graphics *graphics;
        
        

    private:
        Point totalShifts;
        Rect permitedRegion;
    public:
        void setPermitedRegionAndShift(Rect permitedRegionL, Point totalShiftL) {
            totalShifts = totalShiftL;
            permitedRegion = permitedRegionL;
        }
        void clearShifts() {
            totalShifts = Point();
            permitedRegion = Rect(0, 0, size.width, size.height);
        }


    public:
        void startProc();

        void paintBackBuffer(const Rect &rect);

        inline bool resizeBuffer(Size parsize) {
            if (parsize == this->size) {
                return true;
            }
            /*if (parsize.width > (1920 / 100) || parsize.height > (1080 / 100)) {
                if (parsize.width > 1920 || parsize.height > 1080) {
                    this->size = parsize;
                } else {
                    if (this->size == Size(1920, 1080)) {
                        return true;
                    } else {
                        this->size = Size(1920, 1080);
                    }
                }
            } else {
                this->size = parsize;
            }*/
            this->size = parsize;

            //delete backBuffer
            delete[] backBuffer;
            DeleteObject(backBufferHBitmap);

            //delete HDC and graphics
            DeleteDC(backBufferHDC);
            delete graphics;

            //create backBuffer
            backBuffer = new byte[this->size.width * this->size.height * 4];
            backBufferHBitmap = CreateBitmap(this->size.width, this->size.height, 1, 32, backBuffer);

            //create HDC and graphics
            backBufferHDC = CreateCompatibleDC(null);
            SelectObject(backBufferHDC, backBufferHBitmap);
            graphics = new Gdiplus::Graphics(backBufferHDC);
            graphics->FillRectangle(&Gdiplus::SolidBrush(Color(60, 60, 60)), size);
            return false;
            //fillBackground();
        }

        inline void fillBackground(Color color) {
            graphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeNone);
            graphics->FillRectangle(&Gdiplus::SolidBrush(color), size);
        }

        inline void fillRectangle(Rect rect, const Color &color) {
            rect.shiftTo(totalShifts);

            graphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeNone);
            graphics->FillRectangle(&Gdiplus::SolidBrush(color), Rect(rect, permitedRegion));
        }

        inline void drawRectangle(Rect rect, const Color &color) {
            //rect.shiftTo(totalShifts);
            graphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeNone);
            //graphics->DrawRectangle(&Gdiplus::Pen(color), Rect(rect, permitedRegion));
            //graphics->DrawRectangle(&Gdiplus::Pen(color), Rect(rect, permitedRegion));
            drawLine(PointF(rect.x, rect.y), PointF(rect.x + rect.width, rect.y), color, 1);
            drawLine(PointF(rect.x + rect.width, rect.y), PointF(rect.x + rect.width, rect.y + rect.height), color, 1);
            drawLine(PointF(rect.x + rect.width, rect.y + rect.height), PointF(rect.x, rect.y + rect.height), color, 1);
            drawLine(PointF(rect.x, rect.y + rect.height), PointF(rect.x, rect.y), color, 1);
        }

        inline void drawLine(PointF a, PointF b, const Color &color, float width) {
            //no protect
            a += totalShifts;
            b += totalShifts;

            std::vector<PointF> pts;

            float k = (a.y - b.y) / (a.x - b.x);

            float c = a.y - k*a.x;

            if (permitedRegion.contains(a)) {
                pts.push_back(a);
            }

            if (permitedRegion.contains(b)) {
                pts.push_back(b);
            }
            if (a.x > b.x) {
                std::swap(a, b);
            }
            PointF p1(permitedRegion.x, permitedRegion.x*k + c);
            if (permitedRegion.contains(p1) &&(p1.x >= a.x && p1.x <= b.x )) {
                pts.push_back(p1);
            }
            PointF p2(permitedRegion.x + permitedRegion.width - 1, (permitedRegion.x + permitedRegion.width - 1)*k + c);
            if (permitedRegion.contains(p2) && (p2.x >= a.x && p2.x <= b.x)) {
                pts.push_back(p2);
            }
            if (a.y > b.y) {
                std::swap(a, b);
            }
            PointF p3((permitedRegion.y - c) / k, permitedRegion.y);
            if (permitedRegion.contains(p3) && (p3.y >= a.y && p3.y <= b.y)) {
                pts.push_back(p3);
            }

            PointF p4((permitedRegion.y + permitedRegion.height - 1 - c)/ k, (permitedRegion.y + permitedRegion.height - 1));
            if (permitedRegion.contains(p4) && (p4.y >= a.y && p4.y <= b.y)) {
                pts.push_back(p4);
            }


            if (pts.size() < 2) {
                return;
            }


            graphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
            graphics->DrawLine(&Gdiplus::Pen(color, width), pts[0], pts[1]);
        }

        inline void drawLineSP(PointF a, PointF b, const Color &color, float width) {
            //no protect
            a += totalShifts;
            b += totalShifts;

            Rect tmp = permitedRegion;
            permitedRegion.inflate(-2, -2);

            std::vector<PointF> pts;

            float k = (a.y - b.y) / (a.x - b.x);

            float c = a.y - k*a.x;

            if (permitedRegion.contains(a)) {
                pts.push_back(a);
            }

            if (permitedRegion.contains(b)) {
                pts.push_back(b);
            }
            if (a.x > b.x) {
                std::swap(a, b);
            }
            PointF p1(permitedRegion.x, permitedRegion.x*k + c);
            if (permitedRegion.contains(p1) && (p1.x >= a.x && p1.x <= b.x)) {
                pts.push_back(p1);
            }
            PointF p2(permitedRegion.x + permitedRegion.width - 1, (permitedRegion.x + permitedRegion.width - 1)*k + c);
            if (permitedRegion.contains(p2) && (p2.x >= a.x && p2.x <= b.x)) {
                pts.push_back(p2);
            }
            if (a.y > b.y) {
                std::swap(a, b);
            }
            PointF p3((permitedRegion.y - c) / k, permitedRegion.y);
            if (permitedRegion.contains(p3) && (p3.y >= a.y && p3.y <= b.y)) {
                pts.push_back(p3);
            }

            PointF p4((permitedRegion.y + permitedRegion.height - 1 - c) / k, (permitedRegion.y + permitedRegion.height - 1));
            if (permitedRegion.contains(p4) && (p4.y >= a.y && p4.y <= b.y)) {
                pts.push_back(p4);
            }

            permitedRegion = tmp;;
            if (pts.size() < 2) {
                return;
            }


            graphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
            graphics->DrawLine(&Gdiplus::Pen(color, width), pts[0], pts[1]);
        }

        inline void fillEllipse(Rect rect, const Color &color) {
            rect.shiftTo(totalShifts);
            Rect tmp = permitedRegion;
            permitedRegion.inflate(-1, -1);
            graphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
            graphics->FillEllipse(&Gdiplus::SolidBrush(color), Rect(rect, permitedRegion));
            permitedRegion = tmp;
        }

        inline void drawString(
            const String &text,
            const Gdiplus::Font *font,
            const Gdiplus::TextRenderingHint textRenderingHint,
            const Color color,
            Rect rect
        ) 
        {
            rect.shiftTo(totalShifts);
            graphics->SetTextRenderingHint(textRenderingHint);
            Rect rtmp(rect, permitedRegion);
            if (rtmp.isEmptyArea()) {
                return;
            }
            graphics->DrawString(text, -1, font, (Gdiplus::RectF)rtmp, &Gdiplus::StringFormat(), &Gdiplus::SolidBrush(color));
        };


        inline PointF measureString(
            const String &text, 
            const Gdiplus::Font *font,
            const Gdiplus::TextRenderingHint textRenderingHint
        )
        {
            Gdiplus::RectF boundingBox;
            if (this == nullptr) {
                static Gdiplus::Graphics *graphicsTmp = new Gdiplus::Graphics(GetDC(NULL));
                graphicsTmp->SetTextRenderingHint(textRenderingHint);
                graphicsTmp->MeasureString(text, text.size(), font, Gdiplus::PointF(), &boundingBox);
                //delete graphicsTmp;
                return PointF(boundingBox.Width + 1, boundingBox.Height);
            }
            graphics->SetTextRenderingHint(textRenderingHint);
            graphics->MeasureString(text, text.size(), font, Gdiplus::PointF(), &boundingBox);
            return PointF(boundingBox.Width, boundingBox.Height);
        }


    public:
        ~Graphics() {

            //delete backBuffer
            delete[] backBuffer;
            DeleteObject(backBufferHBitmap);

            //delete HDC and graphics
            DeleteDC(backBufferHDC);
            delete graphics;
        }

    };
#endif
}