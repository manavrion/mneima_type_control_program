//заголовочный файл обработчика событий контроллера ардуино

#pragma once
#include "Serial.h"
#include <deque>
#include <functional>
#include <memory>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>

namespace mneima {

    class ArduinoEventConroller {
    public:
        //контруктор и деструктор
        ArduinoEventConroller();
        ~ArduinoEventConroller();

        //методы, которые обеспечивают возможность добавления функторов в определённые списки функторов
        //эти функторы из списков будут вызываться при генерировании соответствующего события
        void AddSensor1EventListener(std::function<void(float)> f) {
            Sensor1EventListener.push_back(move(f));
        }
        void AddSensor2EventListener(std::function<void(float)> f) {
            Sensor2EventListener.push_back(move(f));
        }
        void AddVoltageEventListener(std::function<void(int)> f) {
            VoltageEventListener.push_back(move(f));
        }
        void AddConnectEventListener(std::function<void(void)> f) {
            ConnectEventListeners.push_back(std::move(f));
        }
        void AddDisconnectEventListener(std::function<void(void)> f) {
            DisconnectEventListeners.push_back(std::move(f));
        }

        void SetVoltage(int value) {
            value = max(0, value);
            value = min(230, value);
            SendData(std::to_string(value));
        }

        //вызывает события из списка
        void CallEvents(std::vector<std::function<void(void)>> events) {
            for (auto& f : events) f();
        }

        //отправка сообщения на порт ардуино
        void SendData(std::string str);

        void ChangeDelay(int delay) {
            delayInProgram = delay;
        }

    private:
        volatile int delayInProgram;

        // метод, который запускается только из потока DemonThread
        void Run();

        // метод, который вызывается для парсинга входящего потока символов
        void ParseIncomingData(std::deque<char>& incomingDeque);

        // метод, который вызывается для парсинга строки вида "<data>"
        void ParseData(std::deque<char> incomingDeque);

    private:
        //поток, отвечающий за взаимодействие и прослушку порта, соединённого с ардуино
        std::unique_ptr<std::thread> DemonThread;

        //флаг, поднятие которого будет означать остановку выше указанного потока
        volatile bool BreakOfThread;

        //хранение функторов, которые будут вызываться при появлении соотетствующих событий
        std::vector<std::function<void(void)>>  ConnectEventListeners;
        std::vector<std::function<void(void)>>  DisconnectEventListeners;
        std::vector<std::function<void(float)>> Sensor1EventListener;
        std::vector<std::function<void(float)>> Sensor2EventListener;
        std::vector<std::function<void(int)>>   VoltageEventListener;

        //класс, с реализаций соединения с ардуино
        std::unique_ptr<Serial> SerialPort;

        //очередь сообщений, которые необходимо передать контроллеру
        std::queue<std::string> DataQueue;
        std::mutex DataQueueMutex;  //потоковая защита DataQueue
    };

}