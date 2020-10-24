#include "stdafx.h"
#include "mneima_window.h"
#include "resource.h"

using namespace mneima;
using namespace std;

MneimaWindow::MneimaWindow() 
    : Window(L"Mneima Type Control Program", Size(850, 550)),
    Sensor1Value(-1), Sensor2Value(-1), VoltageValue(-1){

	/*	
        создание строки состояния
		задаём свойства: текст, позиционирование по вертикали и горизонтали, отступы и высоту
		добавляем элемент в MneimaWindow	
     */
    StatusLabel.reset(new Label);
    StatusLabel
        ->SetText(L"Status: Waiting")
        ->SetVerticalAlignment(VerticalAlignment::Bottom)
        ->SetHorizontalAlignment(HorizontalAlignment::Left)
        ->SetMargin({ 5, 5, 5, 5 })
        ->SetHeight(20);
    Add(StatusLabel.get());

    // создание картинки, добавление свойства - прямоугольник отрисовки и добавление в MneimaWindow
    Add((new Image(IDB_PNG_PELTIER))
        ->SetRect({ 350, 350, 256, 128 }));

    /*	
        создание картинки
        задаём свойства: прямоугольник отрисовки и сразу добавляем элемент в MneimaWindow 
     */
    StatusImage.reset(new Image(IDB_PNG_GLOVE_DISCONNECT));
    Add(StatusImage
        ->SetRect({ 350, 16, 256, 256 }));

    /*
        создаем надписи, показывающие температуру сенсоров, вольтаж, среднюю температуру
        задаём свойства: текст и прямоугольник отрисовки
        добавляем элемент в MneimaWindow
    */
    Sensor1Label.reset(new Label);
    Sensor1Label
        ->SetText(L"Sensor 1: --")
        ->SetRect({ 350, 320, 256, 128 });
    Add(Sensor1Label.get());

    Sensor2Label.reset(new Label);
    Sensor2Label
        ->SetText(L"Sensor 2: --")
        ->SetRect({ 350, 490, 256, 128 });
    Add(Sensor2Label.get());

    SensorVoltageLabel.reset(new Label);
    SensorVoltageLabel
        ->SetText(L"Voltage: --")
        ->SetRect({ 10, 350, 256, 128 });
    Add(SensorVoltageLabel.get());

    SensorAverageTemperatureLabel.reset(new Label);
    SensorAverageTemperatureLabel
        ->SetText(L"Average temperature: --")
        ->SetRect({ 10, 370, 256, 128 });
    Add(SensorAverageTemperatureLabel.get());

    /*
        задаём крайние значения для слайдера
        создаём слайдер
        добавляем элемент в MneimaWindow
    */

    // !!! работа при других значениях не гарантируется !!!
    const int MinSliderVal = 15;
    const int MaxSliderVal = 40;
    ControlSlider.reset(new MneimaSlider(MinSliderVal, MaxSliderVal, { 20, 20 }));
    Add(ControlSlider.get());


    // добавляем пульсар
    ControlPulsar.reset(new MneimaPulsar);
    ControlPulsar->SetRect({650, 300, 180, 200});
    Add(ControlPulsar.get());

    // добавляем график
    ControlStat.reset(new MneimaStat);
    ControlStat->SetRect({ 650, 15, 180, 258 });
    Add(ControlStat.get());
    ControlPulsar->AddPulsarListener([=]() {
        ControlStat->AddVoltageSample(VoltageValue);
        ControlStat->AddPreferVoltageSample(PreferVoltageValue + ControlPulsar->GetSinDx());
    });


    // создаём обработчик событий контроллера
	ArduinoBackend.reset(new ArduinoEventConroller());

    // связываем соответствующие слушатели и события
	ArduinoBackend->AddConnectEventListener(bind(&MneimaWindow::ConnectEvent, this));
	ArduinoBackend->AddDisconnectEventListener(bind(&MneimaWindow::DisconnectEvent, this));
	ArduinoBackend->AddSensor1EventListener(bind(&MneimaWindow::Sensor1UpdateEvent, this, std::placeholders::_1));
	ArduinoBackend->AddSensor2EventListener(bind(&MneimaWindow::Sensor2UpdateEvent, this, std::placeholders::_1));
	ArduinoBackend->AddVoltageEventListener(bind(&MneimaWindow::VoltageUpdateEvent, this, std::placeholders::_1));

    // изменение времени опроса устройства
    ControlPulsar->AddPulsarListener([this]() {
        ArduinoBackend->ChangeDelay(ControlPulsar->GetSleepTime());
        SliderUpdaterLogic(VoltageValue);
        update();
    });
}

void MneimaWindow::UpdateAverageTemperature() {
    if (Sensor1Value == -1 || Sensor2Value == -1) return;
    
    String text = L"Average temperature: ";
    auto val = std::to_wstring((Sensor1Value + Sensor2Value) / 2.0);
    val.erase(val.size() - 1 - 4, 4);
    text += val;
    text += L" °C";
    
    SensorAverageTemperatureLabel->SetText(text);
    Sensor1Value = -1;
    Sensor2Value = -1;
}

void MneimaWindow::ConnectEvent() {
    unique_lock<mutex> autoLock(EventMutex);
    //устанавливаем соответствующее изображение перчатки
    StatusImage->setImageFromRes(IDB_PNG_GLOVE_ACTIVE);
    //обновляем строку состояния
    StatusLabel->SetText(L"Status: Connected");
    //обновляем окно
    update();
}

void MneimaWindow::DisconnectEvent() {
    unique_lock<mutex> autoLock(EventMutex);
    //устанавливаем соответствующее изображение перчатки
    StatusImage->setImageFromRes(IDB_PNG_GLOVE_DISCONNECT);
    //обновляем строку состояния
    StatusLabel->SetText(L"Status: Waiting");
    //сбрасываем сенсоры
    Sensor1Label->SetText(L"Sensor 1: --");
    Sensor2Label->SetText(L"Sensor 2: --");
    SensorVoltageLabel->SetText(L"Voltage: --");
    SensorAverageTemperatureLabel->SetText(L"Average temperature: --");
    Sensor1Value = -1;
    Sensor2Value = -1;
    //обновляем окно
    update();
}

void MneimaWindow::Sensor1UpdateEvent(float value) {
    unique_lock<mutex> autoLock(EventMutex);
    //подготовка строки для вывода на статический элемент окна
    String text = L"Sensor 1: ";
    auto val = std::to_wstring(value);
    val.erase(val.size() - 1 - 4, 4);
    text += val;
    text += L" °C";

    Sensor1Label->SetText(text);
    Sensor1Value = value;
    //обновляем среднюю температуру
    UpdateAverageTemperature();
    //обновляем окно
    update();
}

void MneimaWindow::Sensor2UpdateEvent(float value) {
    unique_lock<mutex> autoLock(EventMutex);
    //подготовка строки для вывода на статический элемент окна
    String text = L"Sensor 2: ";
    auto val = std::to_wstring(value);
    val.erase(val.size() - 1 - 4, 4);
    text += val;
    text += L" °C";

    Sensor2Label->SetText(text);
    Sensor2Value = value;
    MainSensorValue = value;
    //обновляем среднюю температуру
    UpdateAverageTemperature();

    //обновление слайдера
    ControlSlider->SetHardwareValue(value);

    //обновляем окно
    update();
}

void MneimaWindow::VoltageUpdateEvent(int value) {
    unique_lock<mutex> autoLock(EventMutex);
    //подготовка строки для вывода на статический элемент окна
    String text = L"Voltage: ";
    auto val = std::to_wstring(value);
    text += val;
    SensorVoltageLabel->SetText(text);

    VoltageValue = value;

    //обновляем окно
    update();
}

void MneimaWindow::SliderUpdaterLogic(float value) {
	auto[minVal, maxVal] = ControlSlider->GetPermSegment();
	if (MainSensorValue < 0) return;
	if (minVal < 0) return;
	if (maxVal < 0) return;

	bool flag = false;
	if (MainSensorValue < minVal && minVal != -1) {
		if (PreferVoltageValue == -1) PreferVoltageValue = value;
		PreferVoltageValue = min(255, PreferVoltageValue - 1);
		flag = true;
	}
	if (MainSensorValue > maxVal && maxVal != -1) {
		if (PreferVoltageValue == -1) PreferVoltageValue = value;
		PreferVoltageValue = max(0, PreferVoltageValue + 1);
		flag = true;
	}

	ArduinoBackend->SetVoltage(PreferVoltageValue + ControlPulsar->GetSinDx());
}
