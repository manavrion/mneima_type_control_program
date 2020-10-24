#pragma once
#include "KeyEvent.h"
#include "Label.h"
namespace MetaFrame {

    class Button : public FrameElement {

    public:
        Button()
            : FrameElement(), 
            label(nullptr) 
        {
            state = ElementState::NORMAL;
            registerMouseEnteredEvent;
            registerMouseExitedEvent;
            registerMousePressedEvent;
            registerMouseReleasedEvent;
        }

        Button *setLabel(String text) {
            if (label == nullptr) {
                label = new Label();
                this->Add(label);
                label->setColor(Color(0, 0, 0))
                    ->SetMargin(10, 10, 10, 10)
                    ->SetHorizontalAlignment(HorizontalAlignment::Center)
                    ->SetVerticalAlignment(VerticalAlignment::Center)
                    ->setAutoHeight(false)
                    ->setAutoWidth(false);
            }
            label->SetText(text);
            this->Pack();
            return this;
        }

        
    protected:

        Color normal = Color(160, 160, 160);
        Color selected = Color(190, 190, 190);
        Color pressed = Color(120, 120, 120);
        Color focused;
        Color disabled = Color(160, 160, 160, 60);

        Label *label;


        void Repaint() {
            Graphics->Clear(Color(0, 0, 0, 0));
            switch (state) {
                case MetaFrame::ElementState::NORMAL:
                    Graphics->FillRectangle(&Gdiplus::SolidBrush(normal), Rect(0, 0, width, height));
                    break;
                case MetaFrame::ElementState::SELECTED:
                    Graphics->FillRectangle(&Gdiplus::SolidBrush(selected), Rect(0, 0, width, height));
                    break;
                case MetaFrame::ElementState::PRESSED:
                    Graphics->FillRectangle(&Gdiplus::SolidBrush(pressed), Rect(0, 0, width, height));
                    break;
                case MetaFrame::ElementState::FOCUSED:
                    Graphics->FillRectangle(&Gdiplus::SolidBrush(focused), Rect(0, 0, width, height));
                    break;
                case MetaFrame::ElementState::DISABLED:
                    Graphics->FillRectangle(&Gdiplus::SolidBrush(disabled), Rect(0, 0, width, height));
                    break;
                default:
                    break;
            }
        }

        //event
        void mouseEntered(const MouseEvent &event, FrameElement *sender) {
            if (enabled) {
                state = ElementState::SELECTED;
                update();
            }
        }
        void mouseExited(const MouseEvent &event, FrameElement *sender) {
            if (enabled) {
                state = ElementState::NORMAL;
                update();
            }
        }
        void mousePressed(const MouseEvent &event, FrameElement *sender) {
            if (enabled) {
                state = ElementState::PRESSED;
                update();
            }
        }
        void mouseReleased(const MouseEvent &event, FrameElement *sender) {
            if (enabled) {
                state = ElementState::SELECTED;
                update();
            }
        }

    public:
        ~Button() {
            if (label != null) {
                delete label;
            }
        }

    };

}