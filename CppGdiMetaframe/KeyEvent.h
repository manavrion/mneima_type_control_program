#pragma once
namespace MetaFrame {

    struct ExtraKeyInfo {
        bool leftAltDown;
        bool rightAltDown;
        bool leftControlDown;
        bool rightControlDown;
        bool leftShiftDown;
        bool rightShiftDown;
    };

    struct KeyEvent {
        KeyEvent() 
            : key(0),
            altDown(false), controlDown(false), shiftDown(shiftDown)
        {}
        wchar key;
        keyCodes code;
        bool altDown;
        bool controlDown;
        bool shiftDown;

    };

    enum class MouseButton {
        NO,
        LEFT,
        MIDDLE,
        RIGHT
    };

    struct MouseEvent {
        MouseEvent(int x, int y) 
            : x(x), y(y), 
            xOnScreen(xOnScreen), yOnScreen(yOnScreen), 
            altDown(false), controlDown(false), shiftDown(false),
            leftButtonDown(false),
            midButtonDown(false),
            rightButtonDown(false),
            causedby(MouseButton::NO),
            xShift(0),
            yShift(0)
        {}
        int x;
        int y;
        int xShift;
        int yShift;
        int xOnScreen;
        int yOnScreen;
        bool altDown;
        bool controlDown;
        bool shiftDown;

        MouseButton causedby;

        bool leftButtonDown;
        bool midButtonDown;
        bool rightButtonDown;
    };
}