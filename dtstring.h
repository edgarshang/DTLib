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
    String(const char* s);
};

}

#endif // DTSTRING_H
