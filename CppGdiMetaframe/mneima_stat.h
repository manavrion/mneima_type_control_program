#pragma once
#include "MetaFrame.h"
#include <memory>
#include <thread>
#include <math.h>
#include <deque>
#include <mutex>

namespace mneima {
    using namespace MetaFrame;
    using namespace std;

    class MneimaStat : public FrameElement {
    public:
        MneimaStat() {}


        void AddVoltageSample(int value) {
            DequeMutex.lock();
            value = max(0, value);
            value = min(255, value);
            VoltageStat.push_back(value);
            DequeMutex.unlock();
        }

        void AddPreferVoltageSample(int value) {
            DequeMutex.lock();
            value = max(0, value);
            value = min(255, value);
            PreferVoltageStat.push_back(value);
            DequeMutex.unlock();
        }

    private:
        void Repaint() {
            // очистка полотна
            Graphics->Clear(Color(0, 0, 255, 30));
            //Graphics->Clear(Color(0, 0, 0, 0));

            Graphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);

            DequeMutex.lock();
            auto it = VoltageStat.rbegin();
            auto jt = PreferVoltageStat.rbegin();
            for (int i = getWidth() - 1; i >= 0 && it != VoltageStat.rend() && jt != PreferVoltageStat.rend(); i -= 2, it++, jt++) {
                int x = i;
                int ys = getHeight() - (*it) - 1;
                int yp = getHeight() - (*jt) - 1;


                Graphics->FillEllipse(&Gdiplus::SolidBrush(Gdiplus::Color::Blue), Rect((x - 1), ys - 1, 3, 3));
                Graphics->FillEllipse(&Gdiplus::SolidBrush(Gdiplus::Color::Red), Rect((x - 1), yp - 1, 3, 3));
            }
            while (VoltageStat.size() > 500) {
                VoltageStat.pop_front();
            }
            DequeMutex.unlock();
        }

    private:
        mutex DequeMutex;
        deque<int> VoltageStat;
        deque<int> PreferVoltageStat;
    };

}

