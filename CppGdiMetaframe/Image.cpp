#include "stdafx.h"
#include "Image.h"
#include <map>

namespace MetaFrame {

    static std::map<std::pair<LPCTSTR, LPCTSTR>, IStream*> stream_map;

    //функция загружает картинку из ресурсов
    IStream* CreateStreamOnResource(LPCTSTR lpName, LPCTSTR lpType) {
        if (stream_map.count({ lpName , lpType }) != 0) {
            return stream_map[{ lpName, lpType }];
        }

        // initialize return value
        IStream * ipStream = NULL;

        // find the resource
        HRSRC hrsrc = FindResource(NULL, lpName, lpType);
        if (hrsrc == NULL)
            return ipStream;

        // load the resource
        DWORD dwResourceSize = SizeofResource(NULL, hrsrc);
        HGLOBAL hglbImage = LoadResource(NULL, hrsrc);
        if (hglbImage == NULL)
            return ipStream;

        // lock the resource, getting a pointer to its data
        LPVOID pvSourceResourceData = LockResource(hglbImage);
        if (pvSourceResourceData == NULL)
            return ipStream;

        // allocate memory to hold the resource data
        HGLOBAL hgblResourceData = GlobalAlloc(GMEM_MOVEABLE, dwResourceSize);
        if (hgblResourceData == NULL)
            return ipStream;

        // get a pointer to the allocated memory
        LPVOID pvResourceData = GlobalLock(hgblResourceData);
        if (pvResourceData == NULL)
            return ipStream;

        // copy the data from the resource to the new memory block
        CopyMemory(pvResourceData, pvSourceResourceData, dwResourceSize);
        GlobalUnlock(hgblResourceData);

        // create a stream on the HGLOBAL containing the data
        if (SUCCEEDED(CreateStreamOnHGlobal(hgblResourceData, TRUE, &ipStream))) {
            if (stream_map.count({ lpName , lpType }) == 0) {
                stream_map[{ lpName, lpType }] = ipStream;
            }
            return ipStream;
        }

        // couldn't create stream; free the memory
        if (ipStream == nullptr) {
            GlobalFree(hgblResourceData);
        }

        if (stream_map.count({ lpName , lpType }) == 0) {
            stream_map[{ lpName, lpType }] = ipStream;
        }

        // no need to unlock or free the resource
        return ipStream;
    }

    Image::Image(int resourceIndex)
        : FrameElement() {
        image.reset(new Gdiplus::Image(CreateStreamOnResource(MAKEINTRESOURCE(resourceIndex), L"PNG")));
    }

    Image* Image::setImageFromRes(int resourceIndex) {
        image.reset(new Gdiplus::Image(CreateStreamOnResource(MAKEINTRESOURCE(resourceIndex), L"PNG")));
        return this;
    }


    void Image::Repaint() {
        Graphics->Clear(Color(0, 0, 0, 0));
        Graphics->DrawImage(image.get(), Rect(0, 0, width, height));
    }

    Image::~Image() {

    }


}