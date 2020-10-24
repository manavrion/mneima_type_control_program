#pragma once
#include "MetaFrame.h"
#include <memory>

namespace mneima {
    using namespace MetaFrame;

    class MneimaSlider : public FrameElement {
    public:
        MneimaSlider(int MinSliderVal, int MaxSliderVal, Point point);

        // возвращает ограничители { min, max }
        std::pair<float, float> GetPermSegment() {
            return { CellMinCurrentValue , CellMaxCurrentValue };
        }

        // обновить температуру
        void SetHardwareValue(float HardwareValue) {
            HardwareValue = min(HardwareValue, MaxSliderVal);
            HardwareValue = max(HardwareValue, MinSliderVal);
            //HardwareCellPos = (HardwareValue - 15) / (40 - 15) * (SliderHeight);
            HardwareCellPos = MinCellPos - ((HardwareValue - 15) / (40 - 15) * (SliderHeight));
            HardwareCellLabel->SetText(L"Hardware cell: " + std::to_wstring(HardwareValue));
        }

        // события обновления соединения
        void DisconnectEvent() {
            HardwareCellPos = 1000; //magic number
        }

    protected:


        void Repaint();

        void MaxCellUpdateEvent() {
            int value = MinCellPos - UserCellMax->GetY();

            double f = (value / double(DxCellPos));
            double t = double(MinSliderVal) + f * abs(MinSliderVal - MaxSliderVal);
            UserCellMaxLabel
                ->SetText(L"Max cell: " + std::to_wstring(t));           

            CellMaxCurrentValue = t;
        }

        void MinCellUpdateEvent() {
            int value = MinCellPos - UserCellMin->GetY();

            double f = (value / double(DxCellPos));
            double t = double(MinSliderVal) + f * abs(MinSliderVal - MaxSliderVal);
            UserCellMinLabel
                ->SetText(L"Min cell: " + std::to_wstring(t));            

            CellMinCurrentValue = t;
        }

        void CellPosCorrection(MetaFrame::Image* cell) {

            int preferedPos = 0;
            int minDy = DxCellPos;

            int i = UserCellWidth;
            while (i <= MinCellPos) {

                int dy = abs(cell->GetY() - i);
                if (minDy > dy) {
                    minDy = dy;
                    preferedPos = i;
                }

                i += 5;
            }

            cell->SetY(preferedPos);
        }


    private:
        // границы слайдера
        const int MinSliderVal;
        const int MaxSliderVal;

        float HardwareCellPos = 1000; //magic number


        // манипуляторы слайдера
        std::unique_ptr<MetaFrame::Image> UserCellMin;
        std::unique_ptr<MetaFrame::Image> UserCellMax;
        // drag support
        std::unique_ptr<MetaFrame::Point> UserCellMinPrePoint;
        std::unique_ptr<MetaFrame::Point> UserCellMaxPrePoint;
        // индикаторы значений манипуляторов
        std::unique_ptr<MetaFrame::Label> UserCellMinLabel;
        std::unique_ptr<MetaFrame::Label> UserCellMaxLabel;
        std::unique_ptr<MetaFrame::Label> HardwareCellLabel;

        // кэшированная кисть для градиента
        std::unique_ptr<Gdiplus::Brush> CachedGradientBrush;
        
        // буферизация метода GetPermSegment, который возвращает ограничители
        float CellMaxCurrentValue = -1;
        float CellMinCurrentValue = -1;

        const int SliderMargin; // отступы от краёв
        const int SliderHeight; // высота слайдера

        const int UserCellWidth;// толщина пользовательских манипуляторов
        const int MaxCellPos;   // максимальная позиция манипуляторов
        const int MinCellPos;   // минимальная позиция манипуляторов
        const int DxCellPos;    // разница между max и min
    };

}