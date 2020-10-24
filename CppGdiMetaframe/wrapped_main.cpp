#include "stdafx.h"
#include "mneima_window.h"

int wrapped_main() {
    
    mneima::MneimaWindow window;

    window.Pack();
    window.Run();

    return 0;
}