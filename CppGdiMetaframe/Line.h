#pragma once

#include "FrameElement.h"

namespace MetaFrame {

    class GraphLine : public FrameElement {
    public:
        GraphLine() : FrameElement() {
            this->SetHorizontalAlignment(HorizontalAlignment::Absolute);
            this->SetVerticalAlignment(VerticalAlignment::Absolute);
            this->setAutoWidth(false);
            this->setAutoHeight(false);
        };


    public:



        PointF getPointOfBegin() {
            x = parent->getX();
            y = parent->GetY();
            width = parent->getWidth();
            height = parent->getHeight();
            return a;
        }
        PointF getPointOfEnd() {
            return b;
        }
        Color getColor() {
            return color;
        }
        float getLineWidth() {
            return lineWidth;
        }

        GraphLine *setPointOfBegin(PointF pointOfBegin) {
            x = parent->getX();
            y = parent->GetY();
            width = parent->getWidth();
            height = parent->getHeight();
            a = pointOfBegin;
            return this;
        }
        GraphLine *setPointOfEnd(PointF pointOfEnd) {
            b = pointOfEnd;
            return this;
        }
        GraphLine *setColor(Color color) {
            this->color = color;
            return this;
        }
        GraphLine *setLineWidth(float lineWidth) {
            this->lineWidth = lineWidth;
            return this;
        }

        PointF a, b;


    private:

        Color color;
        float lineWidth;

        virtual void Repaint() {
            Graphics->Clear(Color(0, 0, 0, 0));
            ///graphics->drawRectangle(Rect(0, 0, width, height), color);
            Graphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
            Graphics->DrawLine(&Gdiplus::Pen(color, lineWidth), a, b);
            //graphics->drawLineSP(PointF(0, 0), PointF(width, height), color, lineWidth);
        };


    public:
        ~GraphLine() {};
    };

}