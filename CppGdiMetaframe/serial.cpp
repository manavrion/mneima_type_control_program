//файл реализации отвечающий за работу с портом ардуино

#include "stdafx.h"
#include "serial.h"
#include <string>
#include <fstream>

const int ARDUINO_WAIT_TIME = 2000;

using namespace mneima;

void Serial::TryOpenConnection(std::wstring portName) throw(InvalidPort, CommStateError) {

    if (hSerial) {
        CloseHandle(this->hSerial);
    }

    hSerial = CreateFile(portName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    //Check if the connection was successfull
    if (this->hSerial == INVALID_HANDLE_VALUE) {
        //If not success full display an Error
        throw InvalidPort{};
    } else {
        //If connected we try to set the comm parameters
        DCB dcbSerialParams = { 0 };

        //Try to get the current
        if (!GetCommState(this->hSerial, &dcbSerialParams)) {
            //If impossible, show an error
            throw CommStateError{};
        } else {
            //Define serial connection parameters for the arduino board
            dcbSerialParams.BaudRate = CBR_9600;
            dcbSerialParams.ByteSize = 8;
            dcbSerialParams.StopBits = ONESTOPBIT;
            dcbSerialParams.Parity = NOPARITY;
            //Setting the DTR to Control_Enable ensures that the Arduino is properly
            //reset upon establishing a connection
            dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

            //Set the parameters and check for their proper application
            if (!SetCommState(hSerial, &dcbSerialParams)) {
                throw CommStateError{};
            } else {
                //Flush any remaining characters in the buffers 
                PurgeComm(this->hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
                //We wait 2s as the arduino board will be reseting
                Sleep(ARDUINO_WAIT_TIME);
            }
        }
    }

}



int Serial::ReadData(char *buffer, unsigned int nbChar) {
    //Number of bytes we'll have read
    DWORD bytesRead;
    //Number of bytes we'll really ask to read
    unsigned int toRead;

    //Use the ClearCommError function to get status info on the Serial port
    ClearCommError(this->hSerial, &this->errors, &this->status);

    //Check if there is something to read
    if (this->status.cbInQue>0) {
        //If there is we check if there is enough data to read the required number
        //of characters, if not we'll read only the available characters to prevent
        //locking of the application.
        if (this->status.cbInQue>nbChar) {
            toRead = nbChar;
        } else {
            toRead = this->status.cbInQue;
        }

        //Try to read the require number of chars, and return the number of read bytes on success
        if (ReadFile(this->hSerial, buffer, toRead, &bytesRead, NULL)) {
            return bytesRead;
        }

    }

    //If nothing has been read, or that an error was detected return 0
    return 0;

}

void Serial::TryWrite(const std::string& data) throw(Disconnected) {
    DWORD bytesSend;
    //Try to write the buffer on the Serial port
    if (!WriteFile(this->hSerial, (void *)data.c_str(), data.size(), &bytesSend, 0)) {
        //In case it don't work get comm error and throw exception
        ClearCommError(this->hSerial, &this->errors, &this->status);
        throw Disconnected();
    } 
}

Serial::~Serial() {
    if (hSerial) {
        //закрытие системного ресурса
        CloseHandle(hSerial);
    }
}