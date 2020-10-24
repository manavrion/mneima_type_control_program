#include "stdafx.h"
#include "Label.h"

namespace MetaFrame {


    Label::Label()
        : FrameElement(),
        text(),
        fontSize(12),
        maxFontSize(12),
        minFontSize(12),
        autoFontSize(false),
        font(new Gdiplus::Font(L"MS Shell Dlg", 10)),
        textRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit),
        color(0, 0, 0) 
    {
        
    }

    String Label::getText() const { return text; }

    inline float Label::getFontSize() const { return fontSize; }

    inline float Label::getMaxFontSize() const { return maxFontSize; }

    inline float Label::getMinFontSize() const { return minFontSize; }

    inline bool Label::getAutoFontSize() const { return autoFontSize; }

    inline Gdiplus::Font *Label::getFont() const { return font; }


    inline Gdiplus::TextRenderingHint Label::getTextRenderingHint() const { return textRenderingHint; }

    inline Color Label::getColor() const { return color; }

    Label *Label::SetText(String text) {
        this->text = text;
        

        Gdiplus::RectF boundingBox;
        static Gdiplus::Graphics *graphicsTmp = new Gdiplus::Graphics(GetDC(NULL));
        graphicsTmp->SetTextRenderingHint(textRenderingHint);
        graphicsTmp->MeasureString(text, text.size(), font, Gdiplus::PointF(), &boundingBox);

        PointF size(boundingBox.Width, boundingBox.Height);


        width = size.x;
        height = size.y;
        return this;
    }

    inline Label * Label::setFontSize(float val) { fontSize = val; return this; }

    inline Label * Label::setMaxFontSize(float val) { maxFontSize = val; return this; }

    inline Label * Label::setMinFontSize(float val) { minFontSize = val; return this; }

    inline Label * Label::setAutoFontSize(bool val) { autoFontSize = val; return this; }

    inline Label * Label::setFont(Gdiplus::Font *val) { font = val; return this; }


    inline Label * Label::setTextRenderingHint(Gdiplus::TextRenderingHint val) { textRenderingHint = val; return this; }

    Label *Label::setColor(Color val) { color = val; return this; }


    void Label::Repaint() {
        Graphics->Clear(Color(0,0,0,0));
        //Graphics->SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintAntiAlias);
        Graphics->DrawString(text, text.size(), font, PointF(0, 0), &Gdiplus::SolidBrush(color));

        //graphics->drawString(text, font, textRenderingHint, color, Rect(0, 0, width, height));
    }

    Label::~Label() {

    }


}