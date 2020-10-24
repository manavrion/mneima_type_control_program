#pragma once

namespace MetaFrame {

    class Label : public FrameElement {
    public:
        Label();

    protected:
        String text;

        float fontSize;
        float maxFontSize;
        float minFontSize;
        bool autoFontSize;

        Gdiplus::Font *font;
        Gdiplus::TextRenderingHint textRenderingHint;

        Color color;


    public:
        String getText() const;
        float getFontSize() const;
        float getMaxFontSize() const;
        float getMinFontSize() const;
        bool getAutoFontSize() const;

        Gdiplus::Font *getFont() const;
        Gdiplus::TextRenderingHint getTextRenderingHint() const;
        Color getColor() const;

        Label *SetText(String val);
        Label *setFontSize(float val);
        Label *setMaxFontSize(float val);
        Label *setMinFontSize(float val);
        Label *setAutoFontSize(bool val);
        Label *setFont(Gdiplus::Font *val);
        Label *setTextRenderingHint(Gdiplus::TextRenderingHint val);
        Label *setColor(Color val);




    protected:
        void Repaint();

    public:
        ~Label();
    };

}