//заголовочный файл отвечающий за работу с портом ардуино

#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace mneima {

    class Serial {
    public:
        // исключения, выбрасываемые данным классом
        // в методах явно прописано через соответствующую спецификацию какие исключения они бросают
        // также имена методов, бросающих исключения начитаются с Try
        class Disconnected : public std::exception {};
        class InvalidPort : public std::exception {};
        class CommStateError : public std::exception {};

        // попытаться установить соединение
        void TryOpenConnection(std::wstring portName) throw(InvalidPort, CommStateError);
        

        //Read data in a buffer, if nbChar is greater than the
        //maximum number of bytes available, it will return only the
        //bytes available. The function return -1 when nothing could
        //be read, the number of bytes actually read.

        //попытаться прочитать из порта
        int ReadData(char *buffer, unsigned int nbChar);

        //попытаться записать в порт
        void TryWrite(const std::string& data) throw(Disconnected);

        ~Serial();

    private:
        //Serial comm handler
        HANDLE hSerial;
        //Get various information about the connection
        COMSTAT status;
        //Keep track of last error
        DWORD errors;
    };

}