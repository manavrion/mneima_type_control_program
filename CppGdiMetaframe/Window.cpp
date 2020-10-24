#include "stdafx.h"
#include "Window.h"
#include <time.h>
namespace MetaFrame {


    Window::Window(String title, Size size, Color background)
        : SystemWindow(title, size, background), FrameElement(), oldSize()
    {
        setBackgroundColor(background);
        this->SetRect(getWindowRect());
        hwndhhhg = hWindow;
        resizeBitMapEvent_newGraphSys();
        //update();
    }

    bool block = false;

    FrameElement *Window::SetRect(Rect rect) {
        x = getWindowRect().x;
        y = getWindowRect().y;
        width = getWindowRect().width;
        height = getWindowRect().height;
        this->setWindowRect(rect);
        return this;
    }

    FrameElement *Window::setLocation(Point p) {
        Rect rect = (this->getRect());
        rect.setPoint(p);
        this->SetRect(rect);
        return this;
    }

    FrameElement *Window::SetSize(Size p) {
        Rect rect = (this->getRect());
        rect.SetSize(p);
        this->SetRect(rect);
        return this;
    }

    void Window::wmPaintBackBufferEvent() {

        int timeOfBeginingPaint = clock();

        checkforvalid();

        //graphics->paintBackBuffer(this->getRect()); //this->rect is wrong
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWindow, &ps);


        //fps
        //int tm = System.timeOfBeginingPaint;
        //Graphics->FillRectangle(&Gdiplus::SolidBrush(Color(0, 0, 80)), Rect(0, 0, 73, 40));
        //Graphics->DrawString(String(clock() - timeOfBeginingPaint) + String(L" ms  \n~") + String(1000 / (clock() - timeOfBeginingPaint + 1)) + String(L" fps"), -1, &Gdiplus::Font(L"Arial", 12), (Gdiplus::RectF)Rect(0, 0, 300, 50), &Gdiplus::StringFormat(), &Gdiplus::SolidBrush(Color(255, 255, 255)));




        //init HDC and graphics
        HBITMAP backBufferHBitmap;

        bitmap->GetHBITMAP(this->backgroundColor, &backBufferHBitmap);

        HDC backBufferHDC = CreateCompatibleDC(null);
        SelectObject(backBufferHDC, backBufferHBitmap);
        BitBlt(hdc, 0, 0, width, height, backBufferHDC, 0, 0, SRCCOPY);


        DeleteObject(backBufferHBitmap);

        //delete HDC and graphics
        DeleteDC(backBufferHDC);

        EndPaint(hWindow, &ps);
    }
    /*
    
    void Window::wmRepaintAll() {
        bool block = true;
        if (System.timeOfBeginingPaint == 0) {
            System.timeOfBeginingPaint = clock();
        }

        graphics->clearShifts();
        this->runRepaintRect(graphics, Point(-x, -y), Rect(x, y, width, height));
        //invalidateScreenRect();
        


        update();
    }*/

    void Window::wmSize(Size &size) {
        //super magic code, don't touch
        this->width = size.width;
        this->height = size.height;
        //graphics->resizeBuffer(size);
        Pack();
        resizeBitMapEvent_newGraphSys();
        /*if (graphics->resizeBuffer(size)) {

            if (oldSize.height >= size.height && oldSize.width < size.width) {
                //-->> <<
                invalidateRect(Rect(oldSize.width, 0, size.width, size.height));

            } else  if (oldSize.height < size.height && oldSize.width >= size.width) {
                //^^
                invalidateRect(Rect(0, oldSize.height, size.width, size.height));

            } else  if (oldSize.height < size.height && oldSize.width < size.width) {
                //^>
                invalidateRect(Rect(oldSize.width, 0, size.width, size.height));
                invalidateRect(Rect(0, oldSize.height, oldSize.width, size.height));

            } else {
                invalidateRect(size.toRect());
            }

        } else {
            invalidateRect(size.toRect());
        }
        */

        oldSize = size;
        //graphics->fillBackground(Color(60, 60, 60));
        //invalidateRect((Rect)size);
        //update();
        
    }

    /*
    void Window::invalidateRect(Rect invalidRect) {
        
        if (System.timeOfBeginingPaint == 0) {
            System.timeOfBeginingPaint = clock();
        }
        
        
        if (!invalidRect.isEmptyArea()) {
            graphics->clearShifts();
            this->runRepaintRect(graphics, Point(-x, -y), invalidRect);
            invalidateScreenRect();
        }
    }

    void Window::update() {
        invalidateRect(Rect(0, 0, width, height));
    }

    

    void Window::updateAsync() {
        UpdateWindow(hWindow);
        //updateAsyncNative();
        //UpdateWindow(hWindow);
        //invalidateScreenRect();
    }

    void Window::Repaint(Graphics *graphics) {
        graphics->fillRectangle(Rect(0, 0, width, height), Color(60, 60, 60));
        //graphics->fillBackground(Color(60, 60, 60));
    }*/




    //event providers

    void Window::wmKeyDown(const KeyEvent &event) {
        this->runKeyPressedEvent(event);
    }

    void Window::wmKeyUp(const KeyEvent & event) {
        this->runKeyReleasedEvent(event);
    }

    void Window::wmKeyTyped(const KeyEvent & event) {
        this->runKeyTypedEvent(event);
    }

    void Window::wmMouseDoubleClick(const MouseEvent & event) {
        this->runMouseDoubleClickedEvent(event);
    }

    void Window::wmMouseEnter(const MouseEvent & event) {
        this->runMouseEnteredEvent(event);
    }

    void Window::wmMouseExit(const MouseEvent & event) {
        this->runMouseExitedEvent(event);
    }

    void Window::wmMouseDrag(const MouseEvent & event) {
        this->runMouseDraggedEvent(event);
    }

    void Window::wmMouseMove(const MouseEvent & event) {
        this->runMouseMovedEvent(event);
    }

    void Window::wmMouseWheelMove(const MouseEvent & event) {
        this->runMouseWheelMovedEvent(event);
    }

    void Window::wmMousePress(const MouseEvent & event) {
        this->runMousePressedEvent(event);
    }

    void Window::wmMouseRelease(const MouseEvent & event) {
        this->runMouseReleasedEvent(event);
    }

    Window::~Window() {

    }
}