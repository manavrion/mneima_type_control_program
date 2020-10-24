#pragma once
#include <functional>
#include "MetaFrame.h"
namespace MetaFrame {

    enum class ElementState {
        NORMAL,
        SELECTED,
        PRESSED,
        FOCUSED,
        DISABLED
    };

#define registerKeyPressedEvent this->addKeyPressedEvent( [this](const KeyEvent &event, FrameElement *sender) { this->keyPressed(event, this); } );
#define registerKeyReleasedEvent this->addKeyReleasedEvent( [this](const KeyEvent &event, FrameElement *sender) { this->keyReleased(event, this); } );
#define registerKeyTypedEvent this->addKeyTypedEvent( [this](const KeyEvent &event, FrameElement *sender) { this->keyTyped(event, this); } );

#define registerMouseDoubleClickedEvent this->addMouseDoubleClickedEvent( [this](const MouseEvent &event, FrameElement *sender) { this->mouseDoubleClicked(event, this); } );
#define registerMouseEnteredEvent this->addMouseEnteredEvent( [this](const MouseEvent &event, FrameElement *sender) { this->mouseEntered(event, this); } );
#define registerMouseExitedEvent this->addMouseExitedEvent( [this](const MouseEvent &event, FrameElement *sender) { this->mouseExited(event, this); } );
#define registerMousePressedEvent this->addMousePressedEvent( [this](const MouseEvent &event, FrameElement *sender) { this->mousePressed(event, this); } );
#define registerMouseReleasedEvent this->addMouseReleasedEvent( [this](const MouseEvent &event, FrameElement *sender) { this->mouseReleased(event, this); } );

#define registerMouseDraggedEvent this->addMouseDraggedEvent( [this](const MouseEvent &event, FrameElement *sender) { this->mouseDragged(event, this); } );
#define registerMouseMovedEvent this->addMouseMovedEvent( [this](const MouseEvent &event, FrameElement *sender) { this->mouseMoved(event, this); } );

#define registerMouseWheelMovedEvent this->addMouseWheelMovedEvent( [this](const MouseEvent &event, FrameElement *sender) { this->mouseWheelMoved(event, this); } );

#define registerAllEvents registerKeyPressedEvent \
registerKeyReleasedEvent \
registerKeyTypedEvent \
registerMouseDoubleClickedEvent \
registerMouseEnteredEvent \
registerMouseExitedEvent \
registerMousePressedEvent \
registerMouseReleasedEvent \
registerMouseDraggedEvent \
registerMouseMovedEvent \
registerMouseWheelMovedEvent


    //typedef void(*KeyFunction) (const KeyEvent &event);
    //typedef void(*MouseFunction) (const MouseEvent &event);
    class FrameElement;
    typedef std::function<void(const KeyEvent &, FrameElement *)> KeyFunction;
    typedef std::function<void(const MouseEvent &, FrameElement *)> MouseFunction;

    class FrameElement {
    public:
        FrameElement();


        //////////////////////////////////////////////
        //render
        Gdiplus::Bitmap *bitmap = nullptr;
        HWND hwndhhhg = nullptr;

        bool isvalid = true;

        virtual void recInv() {
            isvalid = false;
            for (auto &ob : childs) {
                ob->recInv();
            }
        }

        virtual void update() {
            recInv();
            invalidateScreenRectEvent();
        }

        virtual void invalidateScreenRectEvent() {
            parent->invalidateScreenRectEvent();
        }


        virtual void resizeBitMapEvent_newGraphSys() {
            if (hwndhhhg == null) {
                return;
            }

            static Gdiplus::Graphics *graphicsTmp = new Gdiplus::Graphics(hwndhhhg);

            if (width <= 0 || height <= 0) {
                return;
            }


            delete bitmap;
            
            delete Graphics;


            bitmap = new Gdiplus::Bitmap(width, height, graphicsTmp);
            Graphics = new Gdiplus::Graphics(bitmap);
            recrepaint();
        }



        Gdiplus::Graphics *Graphics = nullptr;

        virtual void checkforvalid() {
            for (size_t i = 0; i < childs.size(); i++) {
                childs[i]->checkforvalid();
            }

            if (isvalid == false) {
                recrepaint();
                return;
            } 

            /*for (size_t i = 0; i < childs.size(); i++) {
                childs[i]->checkforvalid();
            }
            if (isvalid == false) {
                Repaint();
                isvalid = true;
                parent->paintBackBuffer();
            }*/
        }

        //ход вниз
        virtual void paintBackBuffer() {
            parent->Graphics->DrawImage(bitmap, getRect());
            parent->paintBackBuffer();
        };

    public:
        //ход вверх
        virtual void recrepaint() {
            if (width == 0 || height == 0) {
                return;
            }
            if (hwndhhhg == null) {
                hwndhhhg = parent->hwndhhhg;
            }
            if (bitmap == null) {
                resizeBitMapEvent_newGraphSys();
            }
            if (bitmap->GetWidth() != width || bitmap->GetHeight() != height) {
                resizeBitMapEvent_newGraphSys();
            }
            Repaint();
            for (size_t i = 0; i < childs.size(); i++) {
                childs[i]->recrepaint();//rewrite!!11
            }
            
            parent->Graphics->DrawImage(bitmap, getRect());
        };

        virtual void Repaint() = 0;

        /////////////////////////////////////////////////////////////////////////////////////////

    protected:
        //location in parent
        int x;
        int y;

        int width;
        int height;
        bool autoWidth;
        bool autoHeight;

        int minWidth;
        int maxWidth;
        int minHeight;
        int maxHeight;

        Margin margin;

        HorizontalAlignment horizontalAlignment;
        VerticalAlignment verticalAlignment;

        Color backgroundColor;

        ElementState state;
        bool enabled = true;

    public:
        virtual FrameElement *setEnabled(bool enabled) {
            this->enabled = enabled;
            if (enabled == true) {
                state = ElementState::NORMAL;
                update();
            } else {
                state = ElementState::DISABLED;
                update();
            }
            return this;
        }


        virtual FrameElement *SetRect(Rect rect);
                     
        virtual FrameElement *setX(int x);
        virtual FrameElement *SetY(int y);
        virtual FrameElement *SetPosition(Point position);
        virtual FrameElement *SetSize(Size size);
        virtual FrameElement *setWidth(int width);
        virtual FrameElement *SetHeight(int height);
        virtual FrameElement *setAutoWidth(bool autoWidth);
        virtual FrameElement *setAutoHeight(bool autoHeight);
        virtual FrameElement *setMinWidth(int minWidth);
        virtual FrameElement *setMaxWidth(int maxWidth);
        virtual FrameElement *setMinHeight(int minHeight);
        virtual FrameElement *setMaxHeight(int maxHeight);
                     
        virtual FrameElement *SetMargin(int left, int right, int top, int bottom);
        virtual FrameElement *SetMargin(Margin margin);
                     
        virtual FrameElement *SetHorizontalAlignment(HorizontalAlignment horizontalAlignment);
        virtual FrameElement *SetVerticalAlignment(VerticalAlignment verticalAlignment);
                     
        virtual FrameElement *setBackgroundColor(Color backgroundColor);

        //getters
        int getX();
        int GetY();

        Rect getRect();

        int getWidth();
        int getHeight();

        Size getSize();
        bool getAutoWidth();
        bool getAutoHeight();
        int getMinWidth();
        int getMaxWidth();
        int getMinHeight();
        int getMaxHeight();

        Margin getMargin();

        HorizontalAlignment getHorizontalAlignment();
        VerticalAlignment getVerticalAlignment();

        Color getBackgroundColor();


        //EVENTS

        virtual void Pack() {
            if (System.timeOfBeginingPaint == 0) {
                System.timeOfBeginingPaint = clock();
            }
            this->runPackEvent();
        };

        virtual void packEvent();

        virtual FrameElement *Add(FrameElement *child);

        virtual void erase(FrameElement *child) {
            for (auto it = childs.begin(); it != childs.end(); it++) {
                if (*it == child) {
                    childs.erase(it);
                    break;
                }
            }
        }

        virtual FrameElement *addToBack(FrameElement *child);

        //virtual void invalidateRect(Rect rect);

        virtual ArrayList<FrameElement*> &getChilds() {
            return childs;
        }

        /*virtual void update() {
            invalidateRect(Rect(-x, -y, width, height));
        }*/

        virtual FrameElement *getParent();

    protected:
        virtual void setParent(FrameElement *parent);


        FrameElement *parent;
        ArrayList<FrameElement*> childs;
        

        Point oldMouseEventPos;

    private:
        ArrayList<KeyFunction>   keyPressedEvents;
        ArrayList<KeyFunction>   keyReleasedEvents;
        ArrayList<KeyFunction>   keyTypedEvents;

        ArrayList<MouseFunction> mouseDoubleClickedEvents;
        ArrayList<MouseFunction> mouseEnteredEvents;
        ArrayList<MouseFunction> mouseExitedEvents;
        ArrayList<MouseFunction> mousePressedEvents;
        ArrayList<MouseFunction> mouseReleasedEvents;

        ArrayList<MouseFunction> mouseDraggedEvents;
        ArrayList<MouseFunction> mouseMovedEvents;

        ArrayList<MouseFunction> mouseWheelMovedEvents;

    public:
        FrameElement *addKeyPressedEvent(KeyFunction f);
        FrameElement *addKeyReleasedEvent(KeyFunction f);
        FrameElement *addKeyTypedEvent(KeyFunction f);
        FrameElement *addMouseDoubleClickedEvent(MouseFunction f);
        FrameElement *addMouseEnteredEvent(MouseFunction f);
        FrameElement *addMouseExitedEvent(MouseFunction f);
        FrameElement *addMousePressedEvent(MouseFunction f);
        FrameElement *addMouseReleasedEvent(MouseFunction f);
        FrameElement *addMouseDraggedEvent(MouseFunction f);
        FrameElement *addMouseMovedEvent(MouseFunction f);
        FrameElement *addMouseWheelMovedEvent(MouseFunction f);

        //////////////
    protected:
        void runKeyPressedEvent(const KeyEvent &event);
        void runKeyReleasedEvent(const KeyEvent &event);
        void runKeyTypedEvent(const KeyEvent &event);

        void runMouseDoubleClickedEvent(MouseEvent event);

        void runMouseEnteredEvent(MouseEvent event);
        void runMouseExitedEvent(MouseEvent event);

        void runMousePressedEvent(MouseEvent event);
        void runMouseReleasedEvent(MouseEvent event);
        void runMouseDraggedEvent(MouseEvent event);
        void runMouseMovedEvent(MouseEvent event);
        void runMouseWheelMovedEvent(MouseEvent event);

        public:
        //virtual void runRepaintRect(Graphics *graphics, Point totalShifts, Rect totalRect);

        void runSizeChanged(const Rect &oldSize, Rect &newSize);

        void runPackEvent();
        //////////

    protected:
        virtual void keyPressed(const KeyEvent &event, FrameElement *sender) {};
        virtual void keyReleased(const KeyEvent &event, FrameElement *sender) {};
        virtual void keyTyped(const KeyEvent &event, FrameElement *sender) {};

        virtual void mouseDoubleClicked(const MouseEvent &event, FrameElement *sender) {};
        virtual void mouseEntered(const MouseEvent &event, FrameElement *sender) {};
        virtual void mouseExited(const MouseEvent &event, FrameElement *sender) {};
        virtual void mousePressed(const MouseEvent &event, FrameElement *sender) {};
        virtual void mouseReleased(const MouseEvent &event, FrameElement *sender) {};

        virtual void mouseDragged(const MouseEvent &event, FrameElement *sender) {};
        virtual void mouseMoved(const MouseEvent &event, FrameElement *sender) {};

        virtual void mouseWheelMoved(const MouseEvent &event, FrameElement *sender) {};

        
        //special
    public:
        


        virtual ~FrameElement();
    };

}