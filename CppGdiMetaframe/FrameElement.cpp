#include "stdafx.h"
#include <windows.h>



namespace MetaFrame {


    FrameElement::FrameElement() 
      : x(0), y(0),
        width(100), height(100),
        autoWidth(false), autoHeight(false),
        minWidth(0), minHeight(0), 
        maxWidth(0x7fffFFFF), maxHeight(0x7fffFFFF),
        margin({10, 10, 10, 10}), 
        horizontalAlignment(HorizontalAlignment::Absolute), 
        verticalAlignment(VerticalAlignment::Absolute),
        parent(null), oldMouseEventPos(),
        backgroundColor(60, 60, 60)
    { 
        //resizeBitMapEvent_newGraphSys();
    }



    FrameElement *FrameElement::SetRect(Rect rect) {
        x = rect.x;
        y = rect.y;
        width = rect.width;
        height = rect.height;
        return this;
    }

    FrameElement *FrameElement::setX(int x) {
        this->x = x;
        return this;
    }

    FrameElement *FrameElement::SetY(int y) {
        this->y = y;
        return this;
    }

    FrameElement * FrameElement::SetPosition(Point position) {
        x = position.x;
        y = position.y;
        return this;
    }

    FrameElement *FrameElement::SetSize(Size size) {
        width = size.width;
        height = size.height;
        resizeBitMapEvent_newGraphSys();
        return this;
    }

    FrameElement *FrameElement::setWidth(int width) {
        this->width = width;
        resizeBitMapEvent_newGraphSys();
        return this;
    }

    FrameElement *FrameElement::SetHeight(int height) {
        this->height = height;
        resizeBitMapEvent_newGraphSys();
        return this;
    }

    FrameElement *FrameElement::setAutoWidth(bool autoWidth) {
        this->autoWidth = autoWidth;
        return this;
    }

    FrameElement *FrameElement::setAutoHeight(bool autoHeight) {
        this->autoHeight = autoHeight;
        return this;
    }

    FrameElement *FrameElement::setMinWidth(int minWidth) {
        this->minWidth = minWidth;
        return this;
    }

    FrameElement *FrameElement::setMaxWidth(int maxWidth) {
        this->maxWidth = maxWidth;
        return this;
    }

    FrameElement *FrameElement::setMinHeight(int minHeight) {
        this->minHeight = minHeight;
        return this;
    }

    FrameElement *FrameElement::setMaxHeight(int maxHeight) {
        this->maxHeight = maxHeight;
        return this;
    }

    FrameElement *FrameElement::SetMargin(int left, int right, int top, int bottom) {
        margin.left = left;
        margin.right = right;
        margin.top = top;
        margin.bottom = bottom;
        return this;
    }

    FrameElement *FrameElement::SetMargin(Margin margin) {
        this->margin = margin;
        return this;
    }

    FrameElement *FrameElement::SetHorizontalAlignment(HorizontalAlignment horizontalAlignment) {
        this->horizontalAlignment = horizontalAlignment;
        return this;
    }

    FrameElement *FrameElement::SetVerticalAlignment(VerticalAlignment verticalAlignment) {
        this->verticalAlignment = verticalAlignment;
        return this;
    }

    FrameElement *FrameElement::setBackgroundColor(Color backgroundColor) {
        this->backgroundColor = backgroundColor;
        return this;
    }

    int FrameElement::getX() {
        return x;
    }

    int FrameElement::GetY() {
        return y;
    }

    Rect FrameElement::getRect() {
        return Rect(x, y, width, height);
    }

    int FrameElement::getWidth() {
        return width;
    }

    int FrameElement::getHeight() {
        return height;
    }

    Size FrameElement::getSize() {
        return Size(width, height);
    }

    bool FrameElement::getAutoWidth() {
        return autoWidth;
    }

    bool FrameElement::getAutoHeight() {
        return autoHeight;
    }

    int FrameElement::getMinWidth() {
        return minWidth;
    }

    int FrameElement::getMaxWidth() {
        return maxWidth;
    }

    int FrameElement::getMinHeight() {
        return minHeight;
    }

    int FrameElement::getMaxHeight() {
        return maxHeight;
    }

    Margin FrameElement::getMargin() {
        return margin;
    }

    HorizontalAlignment FrameElement::getHorizontalAlignment() {
        return horizontalAlignment;
    }

    VerticalAlignment FrameElement::getVerticalAlignment() {
        return verticalAlignment;
    }

    Color FrameElement::getBackgroundColor() {
        return backgroundColor;
    }








    void FrameElement::packEvent() {
        for (auto &child : childs) {

            switch (child->horizontalAlignment) {
                case MetaFrame::HorizontalAlignment::Absolute:
                    break;
                case MetaFrame::HorizontalAlignment::Left:
                {
                    if (child->autoWidth) {


                    } else {
                        child->x = child->margin.left;
                    }
                    break;
                }
                case MetaFrame::HorizontalAlignment::Center:
                {
                    if (child->autoWidth) {
                        child->x = child->margin.left;
                        child->width = width - child->margin.right - child->margin.left;
                    } else {
                        int freeSpace = abs(width - child->width);
                        child->x = freeSpace / 2;
                    }
                    break;
                }
                case MetaFrame::HorizontalAlignment::Right:
                {
                    if (child->autoWidth) {


                    } else {
                        child->x = width - child->width - child->margin.right;
                    }
                    break;
                }
                case MetaFrame::HorizontalAlignment::Stretch:
                {
                    if (child->autoWidth) {
                        child->x = child->margin.left;
                        child->width = width - child->margin.right - child->margin.right;
                    } else {

                    }
                    break;
                }
            }

            switch (child->verticalAlignment) {
                case MetaFrame::VerticalAlignment::Absolute:
                    break;
                case MetaFrame::VerticalAlignment::Top:
                {
                    /*if (child->autoHeight) {
                        child->y = child->margin.top;
                        child->height = height - child->margin.bottom - child->margin.top;
                    } else {
                        int freeSpace = abs(height - child->height);
                        child->y = freeSpace / 2;
                    }*/
                }
                case MetaFrame::VerticalAlignment::Center:
                {
                    if (child->autoHeight) {
                        child->y = child->margin.top;
                        child->height = height - child->margin.bottom - child->margin.top;
                    } else {
                        int freeSpace = abs(height - child->height);
                        child->y = freeSpace / 2;
                    }
                    break;
                }
                case MetaFrame::VerticalAlignment::Bottom:
                {
                    if (child->autoHeight) {
                        /*child->y = child->margin.top;
                        child->height = height - child->margin.bottom - child->margin.top;*/
                    } else {
                        child->y = height - child->height - child->margin.bottom;
                    }
                    break;
                }
                case MetaFrame::VerticalAlignment::Stretch:
                {
                    if (child->autoHeight) {
                        child->y = child->margin.top;
                        child->height = height - child->margin.bottom - child->margin.top;
                    } else {

                    }
                    break;
                }
            }
        }
    }

    FrameElement *FrameElement::Add(FrameElement *child) {
        childs.push_back(child);
        child->setParent(this);
        child->hwndhhhg = hwndhhhg;
        child->resizeBitMapEvent_newGraphSys();
        //child->update();
        return this;
    }

    FrameElement *FrameElement::addToBack(FrameElement * child) {
        childs.insert(childs.begin(), child);
        child->setParent(this);
        child->hwndhhhg = hwndhhhg;
        child->resizeBitMapEvent_newGraphSys();
        //child->update();
        return this;
    }




    /*
    void FrameElement::invalidateRect(Rect rect) {
        if (System.timeOfBeginingPaint == 0) {
            System.timeOfBeginingPaint = clock();
        }
        rect.shiftTo(Point(x, y));
        parent->invalidateRect(Rect(rect, getRect()));
    }*/








    //getters



    FrameElement * FrameElement::getParent() {
        return parent;
    }

    void FrameElement::setParent(FrameElement *parent) {
        this->parent = parent;
    }



















    FrameElement *FrameElement::addKeyPressedEvent(KeyFunction f) {
        keyPressedEvents.push_back(f);
        return this;
    }

    FrameElement *FrameElement::addKeyReleasedEvent(KeyFunction f) {
        keyReleasedEvents.push_back(f);
        return this;
    }

    FrameElement *FrameElement::addKeyTypedEvent(KeyFunction f) {
        keyTypedEvents.push_back(f);
        return this;
    }

    FrameElement *FrameElement::addMouseDoubleClickedEvent(MouseFunction f) {
        mouseDoubleClickedEvents.push_back(f);
        return this;
    }

    FrameElement *FrameElement::addMouseEnteredEvent(MouseFunction f) {
        mouseEnteredEvents.push_back(f);
        return this;
    }

    FrameElement *FrameElement::addMouseExitedEvent(MouseFunction f) {
        mouseExitedEvents.push_back(f);
        return this;
    }

    FrameElement *FrameElement::addMousePressedEvent(MouseFunction f) {
        mousePressedEvents.push_back(f);
        return this;
    }

    FrameElement *FrameElement::addMouseReleasedEvent(MouseFunction f) {
        mouseReleasedEvents.push_back(f);
        return this;
    }

    FrameElement *FrameElement::addMouseDraggedEvent(MouseFunction f) {
        mouseDraggedEvents.push_back(f);
        return this;
    }

    FrameElement *FrameElement::addMouseMovedEvent(MouseFunction f) {
        mouseMovedEvents.push_back(f);
        return this;
    }

    FrameElement *FrameElement::addMouseWheelMovedEvent(MouseFunction f) {
        mouseWheelMovedEvents.push_back(f);
        return this;
    }


    void FrameElement::runKeyPressedEvent(const KeyEvent &event) {
        for (auto &f : keyPressedEvents) {
            (f)(event, this);
        }
        for (auto &ob : childs) {
            ob->runKeyPressedEvent(event);
        }
    }

    void FrameElement::runKeyReleasedEvent(const KeyEvent &event) {
        for (auto &f : keyReleasedEvents) {
            (f)(event, this);
        }
        for (auto &ob : childs) {
            ob->runKeyReleasedEvent(event);
        }
    }

    void FrameElement::runKeyTypedEvent(const KeyEvent &event) {
        for (auto &f : keyTypedEvents) {
            (f)(event, this);
        }
        for (auto &ob : childs) {
            ob->runKeyTypedEvent(event);
        }
    }

    void FrameElement::runMouseDoubleClickedEvent(MouseEvent event) {
        for (auto &f : mouseDoubleClickedEvents) {
            (f)(event, this);
        }
        for (auto &ob : childs) {
            if (Rect(ob->x, ob->y, ob->width, ob->height).contains(Point(event.x, event.y))) {
                MouseEvent tmpEvent = event;
                tmpEvent.x -= ob->x;
                tmpEvent.y -= ob->y;
                tmpEvent.xShift += ob->x;
                tmpEvent.yShift += ob->y;

                ob->runMouseDoubleClickedEvent(tmpEvent);
            }
        }
    }

    void FrameElement::runMouseEnteredEvent(MouseEvent event) {
        for (auto &f : mouseEnteredEvents) {
            (f)(event, this);
        }
        for (auto &ob : childs) {
            if (ob->getRect().contains(Point(event.x, event.y)) && !ob->getRect().contains(oldMouseEventPos)) {
                MouseEvent tmpEvent = event;
                tmpEvent.x -= ob->x;
                tmpEvent.y -= ob->y;
                tmpEvent.xShift += ob->x;
                tmpEvent.yShift += ob->y;
                ob->runMouseEnteredEvent(tmpEvent);
            }
        }

    }

    void FrameElement::runMouseExitedEvent(MouseEvent event) {
        for (auto &f : mouseExitedEvents) {
            (f)(event, this);
        }
        for (auto &ob : childs) {
            if (!ob->getRect().contains(Point(event.x, event.y)) && ob->getRect().contains(oldMouseEventPos)) {
                MouseEvent tmpEvent = event;
                tmpEvent.x -= ob->x;
                tmpEvent.y -= ob->y;
                tmpEvent.xShift += ob->x;
                tmpEvent.yShift += ob->y;
                ob->runMouseExitedEvent(tmpEvent);
            }
        }

    }

    void FrameElement::runMousePressedEvent(MouseEvent event) {
        if (enabled == false) {
            return;
        }
        for (size_t i = 0; i < mousePressedEvents.size(); i++) {
            (mousePressedEvents[i])(event, this);
        }
        for (size_t i = 0; i < childs.size(); i++) {
            if (childs[i]->getRect().contains(Point(event.x, event.y))) {
                MouseEvent tmpEvent = event;
                tmpEvent.x -= childs[i]->x;
                tmpEvent.y -= childs[i]->y;
                tmpEvent.xShift += childs[i]->x;
                tmpEvent.yShift += childs[i]->y;
                childs[i]->runMousePressedEvent(tmpEvent);
            }
        }

    }

    void FrameElement::runMouseReleasedEvent(MouseEvent event) {
        for (auto &f : mouseReleasedEvents) {
            (f)(event, this);
        }
        for (auto &ob : childs) {
            if (Rect(ob->x, ob->y, ob->width, ob->height).contains(Point(event.x, event.y))) {
                MouseEvent tmpEvent = event;
                tmpEvent.x -= ob->x;
                tmpEvent.y -= ob->y;
                tmpEvent.xShift += ob->x;
                tmpEvent.yShift += ob->y;
                ob->runMouseReleasedEvent(tmpEvent);
            }
        }
    }

    void FrameElement::runMouseDraggedEvent(MouseEvent event) {
        for (auto &f : mouseDraggedEvents) {
            (f)(event, this);
        }
        for (size_t i = 0; i < childs.size(); i++) {
            if (Rect(childs[i]->x, childs[i]->y, childs[i]->width, childs[i]->height).contains(Point(event.x, event.y))) {
                MouseEvent tmpEvent = event;
                tmpEvent.x -= childs[i]->x;
                tmpEvent.y -= childs[i]->y;
                tmpEvent.xShift += childs[i]->x;
                tmpEvent.yShift += childs[i]->y;
                childs[i]->runMouseDraggedEvent(tmpEvent);
            }
        }
    }

    void FrameElement::runMouseMovedEvent(MouseEvent event) {
        oldMouseEventPos = Point(event.x, event.y);
        for (auto &f : mouseMovedEvents) {
            (f)(event, this);
        }
        for (auto &ob : childs) {
            if (Rect(ob->x, ob->y, ob->width, ob->height).contains(Point(event.x, event.y))) {
                MouseEvent tmpEvent = event;
                tmpEvent.x -= ob->x;
                tmpEvent.y -= ob->y;
                tmpEvent.xShift += ob->x;
                tmpEvent.yShift += ob->y;
                ob->runMouseMovedEvent(tmpEvent);
            }
        }
    }

    void FrameElement::runMouseWheelMovedEvent(MouseEvent event) {
        for (auto &f : mouseWheelMovedEvents) {
            (f)(event, this);
        }
        for (auto &ob : childs) {
            if (Rect(ob->x, ob->y, ob->width, ob->height).contains(Point(event.x, event.y))) {
                MouseEvent tmpEvent = event;
                tmpEvent.x -= ob->x;
                tmpEvent.y -= ob->y;
                tmpEvent.xShift += ob->x;
                tmpEvent.yShift += ob->y;
                ob->runMouseWheelMovedEvent(tmpEvent);
            }
        }
    }

    /*void FrameElement::runRepaintRect(Graphics *graphics, Point totalShifts, Rect totalRect) {
        
        Rect newTotalRect;

        if (parent == null) {
            newTotalRect = Rect(totalRect, Rect(totalShifts.x, totalShifts.y, width, height));

            graphics->setPermitedRegionAndShift(newTotalRect, totalShifts + Point(x, y));

            newTotalRect = Rect(totalRect, Rect(0, 0, width, height));
        } else {
            newTotalRect = Rect(totalRect, Rect(totalShifts.x, totalShifts.y, parent->width, parent->height));

            graphics->setPermitedRegionAndShift(newTotalRect, totalShifts + Point(x, y));
        }

        totalShifts = totalShifts + Point(x, y);

        this->Repaint(graphics);

        for (auto &ob : childs) {
            
            ob->runRepaintRect(graphics, totalShifts, newTotalRect);
        }

    }*/

    void FrameElement::runSizeChanged(const Rect & oldSize, Rect & newSize) {
    }

    void FrameElement::runPackEvent() {
        this->packEvent();
        for (auto &ob : childs) {
            ob->runPackEvent();
        }
    }



    FrameElement::~FrameElement() {
        childs.clear();
        delete bitmap;
        delete Graphics;
    }

    

}
