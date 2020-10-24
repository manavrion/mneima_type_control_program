#pragma once
#include <windows.h>

namespace MetaFrame {

    struct SystemEventInfo {
        SystemEventInfo(HWND hWindows, UINT message, WPARAM wParam, LPARAM lParam)
            : hWindow(hWindows), message(message), wParam(wParam), lParam(lParam) {
        }
        HWND hWindow;
        UINT message;
        WPARAM wParam;
        LPARAM lParam;
    };


    class SystemWindow {
    public:
        SystemWindow(String title, Size size, Color background);

        String title;           // текст строки заголовка
        String windowClassName; // имя класса главного окна
    protected:
        //Graphics *graphics;

        void invalidateScreenRect(const Rect &rect);
        void invalidateScreenRect();

        void setWindowRect(const Rect &rect);
        Rect getWindowRect();
        HWND hWindow; //дескриптор экземпляра окна
    private:
        HINSTANCE hInstance;    // текущий экземпляр
        HACCEL hAccelTable;     // таблица горячих клавиш



        virtual void wmPaintBackBufferEvent() = 0;
        virtual void wmSize(Size &size) = 0;

        //virtual void wmRepaintAll() = 0;

        virtual void hWindowBind(HWND hWindow) = 0;
         
        //input events
        virtual void wmKeyDown(const KeyEvent &event) = 0;
        virtual void wmKeyUp(const KeyEvent &event) = 0;
        virtual void wmKeyTyped(const KeyEvent &event) = 0;

        virtual void wmMouseDoubleClick(const MouseEvent &event) = 0;
        virtual void wmMouseEnter(const MouseEvent &event) = 0;
        virtual void wmMouseExit(const MouseEvent &event) = 0;
        virtual void wmMousePress(const MouseEvent &event) = 0;
        virtual void wmMouseRelease(const MouseEvent &event) = 0;

        virtual void wmMouseDrag(const MouseEvent &event) = 0;
        virtual void wmMouseMove(const MouseEvent &event) = 0;

        virtual void wmMouseWheelMove(const MouseEvent &event) = 0;


        MouseEvent createMouseEvent(SystemEventInfo eventInfo);
    public:
        int Run();
        int runWithNewStream();




    private:
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        LRESULT WndProcLocal(SystemEventInfo &eventInfo);
    public:
        ~SystemWindow();
    };

}