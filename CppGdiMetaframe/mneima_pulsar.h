#pragma once
#include "MetaFrame.h"
#include <memory>
#include <thread>
#include <math.h>

namespace mneima {
    using namespace MetaFrame;
    using namespace std;

    class MneimaPulsar : public FrameElement {
    public:
        MneimaPulsar() : PulsarThreadKeep(true), SleepTime(50), TimeCounter(0), SinStepMultiplier(1), Deviation(5) {
            SinStep = asin(1) * SinStepMultiplier;

            Add((new Button)
                ->setLabel(L"-")
                ->SetVerticalAlignment(VerticalAlignment::Bottom)
                ->SetSize({20, 20})
                ->addMouseReleasedEvent([this](const MouseEvent &, FrameElement *) { 
                    SleepTime = max(0, SleepTime-1);
                    SleepTimeLabel->SetText(L"SleepTime: " + to_wstring(SleepTime));
                })
            );

            Add((new Button)
                ->setLabel(L"+")
                ->setX(30)
                ->SetVerticalAlignment(VerticalAlignment::Bottom)
                ->SetSize({ 20, 20 })
                ->addMouseReleasedEvent([this](const MouseEvent &, FrameElement *) { 
                    SleepTime = min(1000, SleepTime + 1);
                    SleepTimeLabel->SetText(L"SleepTime: " + to_wstring(SleepTime)); 
                })
            );


            SleepTimeLabel.reset(new Label);
            SleepTimeLabel
                ->SetText(L"SleepTime: " + to_wstring(SleepTime))
                ->setX(60)
                ->SetVerticalAlignment(VerticalAlignment::Bottom);
            Add(SleepTimeLabel.get());


            //SinStepMultiplier

            Add((new Button)
                ->setLabel(L"-")
                ->SetVerticalAlignment(VerticalAlignment::Bottom)
                ->SetMargin(Margin(0, 0, 0, 40))
                ->SetSize({ 20, 20 })
                ->addMouseReleasedEvent([this](const MouseEvent &, FrameElement *) {
                SinStepMultiplier -= 0.05;
                SinStep = asin(1) * SinStepMultiplier;
                MultiplierLabel->SetText(L"Frequency: " + to_wstring(SinStepMultiplier));
            })
            );

            Add((new Button)
                ->setLabel(L"+")
                ->setX(30)
                ->SetVerticalAlignment(VerticalAlignment::Bottom)
                ->SetMargin(Margin(0, 0, 0, 40))
                ->SetSize({ 20, 20 })
                ->addMouseReleasedEvent([this](const MouseEvent &, FrameElement *) {
                SinStepMultiplier += 0.05;
                SinStep = asin(1) * SinStepMultiplier;
                MultiplierLabel->SetText(L"Multiplier: " + to_wstring(SinStepMultiplier));
            })
            );


            MultiplierLabel.reset(new Label);
            MultiplierLabel
                ->SetText(L"Multiplier: " + to_wstring(SinStepMultiplier))
                ->SetMargin(Margin(0, 0, 0, 40))
                ->setX(60)
                ->SetVerticalAlignment(VerticalAlignment::Bottom);
            Add(MultiplierLabel.get());

            //Deviation

            Add((new Button)
                ->setLabel(L"-")
                ->SetVerticalAlignment(VerticalAlignment::Bottom)
                ->SetMargin(Margin(0, 0, 0, 80))
                ->SetSize({ 20, 20 })
                ->addMouseReleasedEvent([this](const MouseEvent &, FrameElement *) {
                Deviation = max(0, Deviation - 1);
                DeviationLabel->SetText(L"Deviation: " + to_wstring(Deviation));
            })
            );

            Add((new Button)
                ->setLabel(L"+")
                ->setX(30)
                ->SetVerticalAlignment(VerticalAlignment::Bottom)
                ->SetMargin(Margin(0, 0, 0, 80))
                ->SetSize({ 20, 20 })
                ->addMouseReleasedEvent([this](const MouseEvent &, FrameElement *) {
                Deviation = min(127, Deviation + 1);
                DeviationLabel->SetText(L"Deviation: " + to_wstring(Deviation));
            })
            );


            DeviationLabel.reset(new Label);
            DeviationLabel
                ->SetText(L"Deviation: " + to_wstring(Deviation))
                ->SetMargin(Margin(0, 0, 0, 80))
                ->setX(60)
                ->SetVerticalAlignment(VerticalAlignment::Bottom);
            Add(DeviationLabel.get());



            //создание и запуск потока
            PulsarThread.reset(new thread(bind(&MneimaPulsar::ThreadFunc, this)));
        };
        ~MneimaPulsar() {
            PulsarThreadKeep = false;
            PulsarThread->join();
        };

        void AddPulsarListener(function<void(void)> listener) {
            PulsarListeners.push_back(move(listener));
        }

        int GetSinDx() {
            return sin(((x)*SinStep + TimeCounter * 8) / 10.0) * Deviation;
        }

        int GetSleepTime() {
            return SleepTime;
        }

    private:
        void Repaint() {
            // очистка полотна
            Graphics->Clear(Color(0, 0, 0, 0));
            /*if (fl) {
                Graphics->Clear(Color(0, 0, 0, 0));
            } else {
                Graphics->Clear(Color(255, 0, 0, 60));
            }*/
            Graphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
            
            const int center = 60;
            const int w = this->getWidth();
            Graphics->DrawLine(&Gdiplus::Pen(Gdiplus::Color::Black), Point(0, 60), Point(w, 60));


            for (int x = 0; x <= w; x += 4) {
                int y = sin(((x)*SinStep + TimeCounter*8) / 10.0) * Deviation;
                if (SinStep < 0.00001) y = 0;
                Graphics->DrawEllipse(&Gdiplus::Pen(Gdiplus::Color::Red), Rect((x - 1), y + center - 1, 3, 3));
            }
            


        }


        void ThreadFunc() {
            while (PulsarThreadKeep) {
                std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
                for (auto& f : PulsarListeners) if(PulsarThreadKeep) f();
                TimeCounter++;

                fl = !fl;
                update();
            }
        }


    private:
        unique_ptr<Label> SleepTimeLabel;
        unique_ptr<Label> MultiplierLabel;
        unique_ptr<Label> DeviationLabel;
        bool fl = true;

        // список слушателей
        vector<function<void()>> PulsarListeners;

        // переменные для реализации потока пульсара
        long long TimeCounter;
        volatile int SleepTime;
        double SinStep;
        double SinStepMultiplier;
        int Deviation;
        unique_ptr<thread> PulsarThread;
        bool PulsarThreadKeep;
    };

}

