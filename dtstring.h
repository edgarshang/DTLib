#ifndef DTSTRING_H
#define DTSTRING_H

#include "object.h"

namespace DTLib {
class String : public Object
{
protected:
    char *m_str;
    int m_length;

    void init(const char* s);
public:
    String();
    String(char c);
    String(const char* s);
    String(const String& s);

    int length() const;
    const char* str() const;

    bool operator == (const String& s) const;
    bool operator == (const char* s) const;

    bool operator != (const String& s) const;
    bool operator != (const char* s) const;

    bool operator > (const String& s) const;
    bool operator > (const char* s) const;

    bool operator < (const String& s) const;
    bool operator < (const char* s) const;

    bool operator <= (const String& s) const;
    bool operator <= (const char* s) const;

    bool operator >= (const String& s) const;
    bool operator >= (const char* s) const;

    String operator +(const String& s) const;
    String operator +(const char* s) const;

    String operator +=(const String& s) const;
    String operator +=(const char* s) const;

    String& operator = (const String& s);
    String& operator = (const char *s);
    String& operator = (char c);
    ~String();
};

}

#endif // DTSTRING_H
