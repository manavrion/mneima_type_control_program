//файл реализации обработчика событий контроллера ардуино

#include "stdafx.h"
#include "arduino_event_conroller.h"
#include "Serial.h"
#include <memory>

using namespace mneima;
using namespace std;

ArduinoEventConroller::ArduinoEventConroller() : BreakOfThread(false), delayInProgram(100) {
    //запуск потока DemonThread, отвечающего за прослушку порта, соединённого с ардуино
    DemonThread.reset(new thread([this]() {
        Run();
    }));
}

ArduinoEventConroller::~ArduinoEventConroller() {
    //поднимаем флаг, чтобы поток остановился DemonThread
    BreakOfThread = true;
    //ждём завершения потока DemonThread
    DemonThread->join();
}

// метод, который запускается только из потока DemonThread
void ArduinoEventConroller::Run() {

    //дек из входящий символов
    deque<char> incomingDeque;

    //число попыток чтения, после которых соединение считается разорванным
    const int failInStream = 100;

    //текущее число проваленных попыток чтения
    int fail = 0;

    //бесконечный цикл пока не будет поднят флаг BreakOfThread
    while (BreakOfThread == false) {
        //задержка
        Sleep(delayInProgram);

        //если соединения ещё не установлено
        if (SerialPort == nullptr) {
            //пытаемся установить соединение
            try {
                //создаём соединение
                SerialPort.reset(new Serial());
                //попытка открыть соединение
                SerialPort->TryOpenConnection(L"\\\\.\\COM5");
                //очищаем очередь исходящих и входящих сообщений DataQueue
                DataQueueMutex.lock();
                while (!DataQueue.empty()) {
                    DataQueue.pop();
                }
                DataQueueMutex.unlock();
                incomingDeque.clear();
                fail = 0;
                if (BreakOfThread) {
                    break;
                }
                //генерируем событие успешного соединения
                CallEvents(ConnectEventListeners);
                continue;
            } catch (const std::exception&) {
                //данный блок кода вызывается при возникновении исключения при попытке открыть соединение

                //генерируем событие разрыва соединения
                CallEvents(DisconnectEventListeners);
                //удаляем SerialPort для пересоздания соединения
                SerialPort.reset();
                continue;
            }
        }

        //данный код делает попытку отправки данных, если DataQueue не пустой
        DataQueueMutex.lock();
        try {            
            while (!DataQueue.empty()) {
                //подготовка строки
                auto message = DataQueue.front();
                DataQueue.pop();
                message += "\n";
                //запись в порт
                SerialPort->TryWrite(message);
            }
        } catch (const std::exception&) {
            //данный блок кода вызывается при возникновении исключения при попытке записи в порт

            //генерируем событие разрыва соединения
            CallEvents(DisconnectEventListeners);
            //удаляем SerialPort для пересоздания соединения
            SerialPort.reset();
            
            continue;
        }
        DataQueueMutex.unlock();
        
        
        //данный код делает попытку чтения данных
        try {
            //подготовка буферов
            char incomingData[256];
            int dataLength = 255;
            int readResult = 0;

            //попытка чтения из порта
            readResult = SerialPort->ReadData(incomingData, dataLength);

            //внесение прочитанных символов во входящую очередь данных
            for (size_t i = 0; i < readResult; i++) {
                incomingDeque.push_back(incomingData[i]);
            }

            //парсинг входящих данных
            ParseIncomingData(incomingDeque);

            //если readResult == 0 значит попытка чтения не удалась
            if (readResult == 0) {
                fail++;
            } else {
                fail = 0;
            }

        } catch (const std::exception&) {
            //данный блок кода вызывается при возникновении исключения при попытке чтения из порта

            //генерируем событие разрыва соединения
            CallEvents(DisconnectEventListeners);
            //удаляем SerialPort для пересоздания соединения
            SerialPort.reset();
        }

        //если количество неуданых попыток чтения превысит failInStream, то соединение будет считаться разорванным
        if (fail >= failInStream) {
            //генерируем событие разрыва соединения
            CallEvents(DisconnectEventListeners);
            //удаляем SerialPort для пересоздания соединения
            SerialPort.reset();
        }

    }

}

// метод, который вызывается для парсинга входящего потока символов
void ArduinoEventConroller::ParseIncomingData(deque<char>& incomingDeque) {
    //данный метод ищет строки вида <data> во входящем потоке
    //в buf будем складывать строки вида <data>, строка при этом может быть не полной
    deque<char> buf;
    //флаг, обозначающий что мы находимся внутри скобок <>
    bool open = false;

    //цикл по каждому символу из входящей очереди
    for (auto ch : incomingDeque) {


        if (open) {
            //если мы находимся внутри скобок <>

            if (ch == '>') {
                //если очередной символ равен >
                //кладём символ в буфер
                buf.push_back(ch);
                //вызываем ParseData, так как у нас сформирована строка вида <data>
                ParseData(buf);
                //сбрасываем буфер и говорим что мы уже не внутри <>
                buf.clear();
                open = false;
            } else {
                //кладём символ в буфер
                buf.push_back(ch);
            }
        } else {
            if (ch == '<') {
                //если очередной символ равен <
                //кладём символ в буфер и объявляем что мы внутри скобок <>
                buf.push_back(ch);
                open = true;
            } else {
                //эти данные игнорируются
            }
        }
    }
    //присваиваем входящей очереди буфер, так как в буфере может находиться не полная строка вида <data>, которая дополнится при очередном считывании с порта
    incomingDeque = buf;
}

// метод, который вызывается для парсинга строки вида "<data>"
void ArduinoEventConroller::ParseData(deque<char> incomingData) {
    //удаляем символы скобок
    incomingData.pop_back();
    incomingData.pop_front();

    //переводим в строку
    string str(incomingData.begin(), incomingData.end());

    //префикс 's' обозначает сенсор
    if (str[0] == 's') {

        //префикс "s1" обозначает сенсор 1
        if (str[1] == '1') {
            //удаляем 3 символа, так как префикс равен "s1_"
            str.erase(0, 3);
            //переводим значение в число
            float value = atof(str.c_str());
            //генерируем соответствующее событие
            for (auto& f : Sensor1EventListener) {
                f(value);
            }
        }

        //префикс "s1" обозначает сенсор 2
        if (str[1] == '2') {
            // удаляем 3 символа, так как префикс равен "s2_"
            str.erase(0, 3);
            //переводим значение в число
            float value = atof(str.c_str());
            //генерируем соответствующее событие
            for (auto& f : Sensor2EventListener) {
                f(value);
            }
        }

    } else if (str[0] == 'v') {
        //префикс 'v' обозначает вольтаж

        // удаляем 2 символа, так как префикс равен "v_"
        str.erase(0, 2);
        //переводим значение в число
        int value = atoi(str.c_str());
        //генерируем соответствующее событие
        for (auto& f : VoltageEventListener) {
            f(value);
        }        
    } else {
        //данный код не должен никогда вызываться
        //TODO: нужно сделать логгирование
        auto s = "unknown data";
    }
}

void ArduinoEventConroller::SendData(string str) {
    DataQueueMutex.lock();
    //кладём сообщение str в очередь данных для отправки
    DataQueue.push(str);
    DataQueueMutex.unlock();
}
