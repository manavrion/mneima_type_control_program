#pragma once
#include <process.h>

//stl
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <list>

#define null nullptr

#define infoOS win
#define infoGraph gdi

#if (infoOS == win)
    #include <windows.h>
    #include "resource.h"
#endif

#if (infoGraph == gdi)
    //GdiPlus connecting
   /* #include <objidl.h>
    #include <gdiplus.h>
    #pragma comment (lib,"Gdiplus.lib")*/
#endif
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")


#include "System.h"
//Metaframe
#include "MetaTypes.h"

//Metaframe classes
#include "InputCodes.h"
#include "KeyEvent.h"
#include "Graphics.h"
#include "FrameElement.h"
#include "SystemWindow.h"
#include "Window.h"

#include "Label.h"
#include "Panel.h"
#include "Table.h"
#include "Line.h"
#include "Button.h"

#include "Image.h"