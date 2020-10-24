#include "stdafx.h"
#include "mneima_slider.h"
#include "Image.h"

using namespace mneima;
using namespace std;

MneimaSlider::MneimaSlider(int minSliderVal, int maxSliderVal, Point point) : 
    MinSliderVal(minSliderVal), 
    MaxSliderVal(maxSliderVal),
    SliderMargin(10),   // отступы от краёв
    SliderHeight(abs(MinSliderVal - MaxSliderVal) * 10), // высота слайдера
    UserCellWidth(3),   // толщина пользовательских манипуляторов
    MaxCellPos(UserCellWidth),  // максимальная позиция манипуляторов
    MinCellPos(SliderHeight + UserCellWidth),
    DxCellPos(abs(MaxCellPos - MinCellPos))
{
    // размещение MneimaSlider
    SetPosition(point);
    SetSize({ 300, SliderMargin * 2 + SliderHeight });

    // создание надписи о минимальном значении и размещении в MneimaSlider
    Add((new Label)
        ->SetText(to_wstring(MinSliderVal) + L" °C")
        ->SetPosition({0, height - 20}));

    // создание надписи о максимальном значении и размещении в MneimaSlider
    Add((new Label)
        ->SetText(to_wstring(MaxSliderVal) + L" °C")
        ->SetPosition({ 0, 0 }));


    /*
        создание надписи о минимальном манипуляторе слайдера
        задаём свойства: текст, позицию
        добавляем элемент в MneimaSlider
    */
    UserCellMinLabel.reset(new Label);
    UserCellMinLabel
        ->SetText(L"Min cell: ")
        ->SetPosition({ 150, 140 });
    Add(UserCellMinLabel.get());

    /*
        создание надписи о максимальном манипуляторе слайдера
        задаём свойства: текст, позицию
        добавляем элемент в MneimaSlider
    */
    UserCellMaxLabel.reset(new Label);
    UserCellMaxLabel
        ->SetText(L"Max cell: ")
        ->SetPosition({150, 120});
    Add(UserCellMaxLabel.get());

    /*
    создание надписи о максимальном манипуляторе слайдера
    задаём свойства: текст, позицию
    добавляем элемент в MneimaSlider
    */
    HardwareCellLabel.reset(new Label);
    HardwareCellLabel
        ->SetText(L"Hardware cell: ")
        ->SetPosition({ 150, 160 });
    Add(HardwareCellLabel.get());

    /*
        создание картинок манипуляторов
        установка их размеров
        установка их позиций
        добавляем элементов в MneimaSlider
    */
    UserCellMax.reset(new Image(IDB_PNG_CELL));
    UserCellMin.reset(new Image(IDB_PNG_CELL));
    UserCellMax->SetSize({120, 15});
    UserCellMin->SetSize({ 120, 15 });
    UserCellMax->SetPosition({ 40 - 2, UserCellWidth });
    UserCellMin->SetPosition({ 40 - 2, SliderHeight + UserCellWidth });
    Add(UserCellMax.get());
    Add(UserCellMin.get());



    UserCellMax->addMousePressedEvent([this](const MouseEvent& event, FrameElement* cell) {
        // создаём UserCellMaxPrePoint, этот объект существует пока зажата кнопка мыши
        UserCellMaxPrePoint.reset(new Point{ event.x, event.y });
    });
    UserCellMax->addMouseMovedEvent([=](const MouseEvent& event, FrameElement* cell) {
        if (UserCellMaxPrePoint == nullptr) return;        
        // извлечение сдвига
        Point shift(UserCellMaxPrePoint->x - event.x, 
            UserCellMaxPrePoint->y - event.y);

        int currentPos = cell->GetY() - shift.y;
        // позиции манипуляторов не превысят установленные границы
        currentPos = max(currentPos, MaxCellPos);
        currentPos = min(currentPos, MinCellPos);
        // один манипулятор всегда будет выше другого
        currentPos = min(currentPos, UserCellMin->GetY() - 10);
        // задаём позицию
        cell->SetY(currentPos);
        
        // создаём событие манипулятора и обновляем MneimaSlider
        MaxCellUpdateEvent();
        update();
    });

    UserCellMin->addMousePressedEvent([this](const MouseEvent& event, FrameElement* cell) {
        // создаём UserCellMaxPrePoint, этот объект существует пока зажата кнопка мыши
        UserCellMinPrePoint.reset(new Point{ event.x, event.y });
    });
    UserCellMin->addMouseMovedEvent([=](const MouseEvent& event, FrameElement* cell) {
        if (UserCellMinPrePoint == nullptr) return;
        // извлечение сдвига
        Point shift(UserCellMinPrePoint->x - event.x,
            UserCellMinPrePoint->y - event.y);

        int currentPos = cell->GetY() - shift.y;
        // позиции манипуляторов не превысят установленные границы
        currentPos = max(currentPos, MaxCellPos);
        currentPos = min(currentPos, MinCellPos);
        // один манипулятор всегда будет выше другого
        currentPos = max(currentPos, UserCellMax->GetY() + 10);
        // задаём позицию
        cell->SetY(currentPos);

        // создаём событие манипулятора и обновляем MneimaSlider
        MinCellUpdateEvent();
        update();
    });

    addMouseReleasedEvent([this](const MouseEvent& e, FrameElement* cell) {
        // удаляем UserCellMinPrePoint и UserCellMaxPrePoint, 
        // так как эти объекты существует пока зажата кнопка мыши
        UserCellMinPrePoint.reset();
        UserCellMaxPrePoint.reset();
        // корректируем манипуляторы
        CellPosCorrection(UserCellMin.get());
        CellPosCorrection(UserCellMax.get());
        // вызываем события изменения
        MinCellUpdateEvent();
        MaxCellUpdateEvent();
        // обновление MneimaSlider
        update();
    });


    // создание кэшированной кисти для градиента
    CachedGradientBrush.reset(new Gdiplus::LinearGradientBrush(
        Point(0, 0), Point(0, height),
        Color(255, 0, 0), Color(0, 0, 255)));
}

void MneimaSlider::Repaint() {
    // очистка полотна
    //Graphics->Clear(Color(255, 0, 0, 60));
    Graphics->Clear(Color(0, 0, 0, 0));
    
    // рисование градиента
    Graphics->FillRectangle(CachedGradientBrush.get(), 
        Rect(40, SliderMargin, 80, SliderHeight + 1));

    // рисование HardwareCell
    Graphics->FillRectangle(
        &Gdiplus::SolidBrush(Color(0, 0, 0)),
        Rect(40, 9 + HardwareCellPos, 80, 3));
}
