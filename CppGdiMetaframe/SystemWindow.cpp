#include "stdafx.h"
#include "SystemWindow.h"
#include <Windowsx.h>
namespace MetaFrame {
    
    static HashMap<HWND, SystemWindow*> windowMapHH;

    LRESULT CALLBACK SystemWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        return windowMapHH[hWnd]->WndProcLocal(SystemEventInfo(hWnd, message, wParam, lParam));
    }

    LRESULT SystemWindow::WndProcLocal(SystemEventInfo &eventInfo) {
        //WARNING pointer 'this' can be NULL!!11111 
        switch (eventInfo.message) {
            case WM_PAINT:
            {
                if (this == null) return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                //wmRepaintAll();

                wmPaintBackBufferEvent();
                break;
            }
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                break;
            }
            case WM_DISPLAYCHANGE: case WM_ERASEBKGND:
            {
                InvalidateRect(eventInfo.hWindow, NULL, FALSE);//fix graphic
                //return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                break;
            }
            case WM_SIZE:
            {
                if (this == null) return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                wmSize(Size(LOWORD(eventInfo.lParam), HIWORD(eventInfo.lParam)));
                InvalidateRect(hWindow, NULL, FALSE);
                break;
            }
            case WM_KEYDOWN:
            {
                if (this == null) return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                KeyEvent event;
                event.code = static_cast<keyCodes>(eventInfo.wParam); //Code

                BYTE lpKeyState[256];
                GetKeyboardState(lpKeyState);
                ToUnicode(eventInfo.wParam, HIWORD(eventInfo.lParam) & 0xFF, lpKeyState, &event.key, 1, 0);
                
                if (eventInfo.lParam && (0x1 << 30) == 0) {
                    this->wmKeyDown(event);
                }
                this->wmKeyTyped(event);
                break;
            }
            case WM_KEYUP:
            {
                if (this == null) return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                KeyEvent event;
                event.code = static_cast<keyCodes>(eventInfo.wParam); //Code

                BYTE lpKeyState[256];
                GetKeyboardState(lpKeyState);
                ToUnicode(eventInfo.wParam, HIWORD(eventInfo.lParam) & 0xFF, lpKeyState, &event.key, 1, 0);
                this->wmKeyUp(event);

                break;
            }
            case WM_MOUSEMOVE: 
            {
                if (this == null) return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                MouseEvent event(createMouseEvent(eventInfo));
                //alt todo

                if (event.leftButtonDown) {
                    this->wmMouseDrag(event);
                }
                this->wmMouseEnter(event);
                this->wmMouseExit(event);

                this->wmMouseMove(event);
                break;
            }
            case WM_LBUTTONDOWN:
            {
                if (this == null) return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                MouseEvent event(createMouseEvent(eventInfo));
                //alt todo
                event.causedby = MouseButton::LEFT;
                this->wmMousePress(event);
                break;
            }
            case WM_LBUTTONUP:
            {
                if (this == null) return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                MouseEvent event(createMouseEvent(eventInfo));
                //alt todo
                event.causedby = MouseButton::LEFT;
                this->wmMouseRelease(event);
                break;
            }
            case WM_LBUTTONDBLCLK:
            {
                if (this == null) return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                MouseEvent event(createMouseEvent(eventInfo));
                //alt todo
                event.causedby = MouseButton::LEFT;
                this->wmMouseDoubleClick(event);
                this->wmMousePress(event);
                break;
            }
            case WM_RBUTTONDOWN:
            {
                if (this == null) return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                MouseEvent event(createMouseEvent(eventInfo));
                //alt todo
                event.causedby = MouseButton::RIGHT;
                this->wmMousePress(event);
                break;
            }
            case WM_RBUTTONUP:
            {
                if (this == null) return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                MouseEvent event(createMouseEvent(eventInfo));
                //alt todo
                event.causedby = MouseButton::RIGHT;
                this->wmMouseRelease(event);
                break;
            }
            case WM_RBUTTONDBLCLK:
            {
                if (this == null) return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                MouseEvent event(createMouseEvent(eventInfo));
                //alt todo
                event.causedby = MouseButton::RIGHT;
                this->wmMouseDoubleClick(event);
                this->wmMousePress(event);
                break;
            }
            case WM_MBUTTONDOWN:
            {
                if (this == null) return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                MouseEvent event(createMouseEvent(eventInfo));
                //alt todo
                event.causedby = MouseButton::MIDDLE;
                this->wmMousePress(event);
                break;
            }
            case WM_MBUTTONUP:
            {
                if (this == null) return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                MouseEvent event(createMouseEvent(eventInfo));
                //alt todo
                event.causedby = MouseButton::MIDDLE;
                this->wmMouseRelease(event);
                break;
            }
            case WM_MBUTTONDBLCLK:
            {
                if (this == null) return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
                MouseEvent event(createMouseEvent(eventInfo));
                //alt todo
                event.causedby = MouseButton::MIDDLE;
                this->wmMouseDoubleClick(event);
                this->wmMousePress(event);
                break;
            }
            default:
                return DefWindowProc(eventInfo.hWindow, eventInfo.message, eventInfo.wParam, eventInfo.lParam);
        }
        return 0;
    }





    SystemWindow::SystemWindow(String title, Size size, Color background) 
        : windowClassName(), hInstance(), title(title)
    {
        for (auto ch : title) {
            if (isalpha(ch)) {
                windowClassName.push_back(ch);
            }
        }


        HINSTANCE hInstance = GetModuleHandle(nullptr);
        //регистраци¤ класса окна
        WNDCLASSEXW wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_METAFRAME));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
#if _DEBUG
        wcex.hbrBackground = null;
#else
        wcex.hbrBackground = CreateSolidBrush(RGB(background.red, background.green, background.blue));
#endif
        //wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_METAFRAME);
        wcex.lpszMenuName = null;

        wcex.lpszClassName = windowClassName;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        RegisterClassExW(&wcex);

        //сохран¤ет обработку экземпл¤ра и создает главное окно.
        //InitInstance
        //        ¬ данной функции дескриптор экземпл¤ра сохран¤етс¤ в глобальной переменной, а также
        //        создаетс¤ и выводитс¤ на экран главное окно программы.
        //WS_OVERLAPPEDWINDOW
        hWindow = CreateWindowW(windowClassName, title, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, size.width + 16, size.height + 39, nullptr, nullptr, hInstance, nullptr);

        //lock resiziong
        LONG style;
        style = GetWindowLong(hWindow, GWL_STYLE);
        style &= ~WS_SIZEBOX;
        SetWindowLong(hWindow, GWL_STYLE, style);


        if (!hWindow) {
            exit(1);
        }
        
        hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_METAFRAME));

        //graph init
        //graphics = new Graphics(size, background, hWindow);
        //graphics->fillRectangle(Rect(10, 10, 200, 200), Color(50, 50, 50));
    }

    void SystemWindow::invalidateScreenRect(const Rect &rect) {
        InvalidateRect(hWindow, &RECT(rect), FALSE);
    }

    void SystemWindow::invalidateScreenRect() {
        InvalidateRect(hWindow, NULL, FALSE);
    }

    void SystemWindow::setWindowRect(const Rect & rect) {
        MoveWindow(hWindow, rect.x, rect.y, rect.width + 16, rect.height + 39, false);
    }

    Rect SystemWindow::getWindowRect() {
        RECT p;
        GetWindowRect(hWindow, &p);
        Rect r(p);
        r.width -= 16;
        r.height -= 39;
        return r;
    }


    inline MouseEvent SystemWindow::createMouseEvent(SystemEventInfo eventInfo) {
        MouseEvent event(GET_X_LPARAM(eventInfo.lParam), GET_Y_LPARAM(eventInfo.lParam));
        if (eventInfo.wParam & MK_CONTROL) event.controlDown = true;
        if (eventInfo.wParam & MK_LBUTTON) event.leftButtonDown = true;
        if (eventInfo.wParam & MK_MBUTTON) event.midButtonDown = true;
        if (eventInfo.wParam & MK_RBUTTON) event.rightButtonDown = true;
        if (eventInfo.wParam & MK_SHIFT)   event.shiftDown = true;
        return event;
    }

    int SystemWindow::Run() {
        bool nCmdShow = true;
        windowMapHH[hWindow] = this;
        
        this->hWindowBind(hWindow);
        ShowWindow(hWindow, nCmdShow);
        UpdateWindow(hWindow);

        MSG msg;
        // ÷икл основного сообщени¤:
        while (GetMessage(&msg, nullptr, 0, 0)) {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        return (int)msg.wParam;
    }

    int SystemWindow::runWithNewStream() {

        _beginthread([](void* pParams) {
            ((SystemWindow*)pParams)->Run();
        }, 0, this);

        return 0;
    }





    SystemWindow::~SystemWindow() {
        //delete graphics;
        windowMapHH.erase(hWindow);
    }


}