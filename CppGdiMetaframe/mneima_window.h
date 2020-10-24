#pragma once
#include <memory>
#include <mutex>

#include "arduino_event_conroller.h"
#include "MetaFrame.h"
#include "mneima_pulsar.h"
#include "mneima_slider.h"
#include "mneima_stat.h"

namespace mneima {
	using namespace MetaFrame;

    class MneimaWindow : public Window {
    public:
        MneimaWindow();
    private:
        void UpdateAverageTemperature();

		void ConnectEvent();
		void DisconnectEvent();

        void Sensor1UpdateEvent(float value);
        void Sensor2UpdateEvent(float value);
        void VoltageUpdateEvent(int value);


        void SliderUpdaterLogic(float value);

    private:
        // пульсар
        std::unique_ptr<MneimaPulsar> ControlPulsar;

        // элементы отвечающие за информирование состояния подключения
        std::unique_ptr<Label> StatusLabel;
        std::unique_ptr<Image> StatusImage;
        
        // показатели сенсоров
        std::unique_ptr<Label> Sensor1Label;
        std::unique_ptr<Label> Sensor2Label;
        std::unique_ptr<Label> SensorVoltageLabel;
        std::unique_ptr<Label> SensorAverageTemperatureLabel;

        // слайдер
        std::unique_ptr<MneimaSlider> ControlSlider;

        // график
        std::unique_ptr<MneimaStat> ControlStat;
      
        // обработчик событий контроллера ардуино
        std::unique_ptr<ArduinoEventConroller> ArduinoBackend;

        // мьютекс для потокобезопасности событий приходящих от ArduinoBackend
        std::mutex EventMutex;

        // сохранение результатов сенсоров
        float Sensor1Value;
        float Sensor2Value;
        float MainSensorValue;
        int VoltageValue;

        // шаг
        int PreferVoltageValue = 0;
    };

}