#pragma once
#include <memory>
namespace MetaFrame {

    class Image : public FrameElement {
    public:
        Image(int resourceIndex);

        Image* setImageFromRes(int resourceIndex);

    protected:
        std::unique_ptr<Gdiplus::Image> image;

    protected:
        void Repaint();

    public:
        ~Image();
    };

}