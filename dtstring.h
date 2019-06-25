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

    ~String();
};

}

#endif // DTSTRING_H
