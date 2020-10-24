#pragma once
#include <string>
namespace MetaFrame {

    //standart types
    typedef unsigned char byte;
    typedef wchar_t wchar;

    class String : public std::wstring {
    public:
        String();
        String(const wchar *a);
        String(const std::wstring a);
        String(long long ll);
        operator const wchar*() const;

        friend String operator+(const String &s1, const String &s2);
        friend String operator+(const String &s1, const wchar *a);
        friend String operator+(const wchar *a, const String &s1);

        int toValueInt();
    };

    typedef std::string ByteString;

}
