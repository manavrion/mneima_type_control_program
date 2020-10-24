#include "stdafx.h"
#include "String.h"

namespace MetaFrame {

    String::String() : std::wstring() {}

    String::String(const wchar *a) : std::wstring(a) {}

    String::String(const std::wstring a) : std::wstring(a) {}

    String::String(long long ll) {
        wchar buffer[32];
        _itow(ll, buffer, 10);
        this->operator=(buffer);
    }

    String::operator const wchar*()const {
        return this->c_str();
    }

    int String::toValueInt() {
        return _wtoi(this->c_str());
    }

    String operator+(const String &s1, const String &s2) {
        return String(std::wstring(s1.c_str()) + std::wstring(s2.c_str()));
    }

    String operator+(const String & s1, const wchar * a) {
        return s1 + String(a);
    }

    String operator+(const wchar * a, const String & s1) {
        return String(a) + String(s1);
    }


}