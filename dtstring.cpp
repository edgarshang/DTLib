#include "dtstring.h"
#include <cstring>
#include <cstdlib>
#include "exception.h"

using namespace std;
namespace DTLib {

void String::init(const char *s)
{
    m_str = strdup(s);

    if( m_str)
    {
        m_length = (int)strlen(m_str);
    }else {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create String object...");
    }
}

String::String()
{
    init("");
}

String::String(const char* s)
{
    init(s ? s : "");
}

String::String(const String& s)
{
    init(s.m_str);
}

String::String(char c)
{
    char s[] = {c, '\0'};

    init(s);
}

int String::length() const
{
    return m_length;
}

const char* String::str() const
{
    return m_str;
}
String::~String()
{
    free(m_str);
}

bool String::operator == (const String& s) const
{
    return (strcmp(m_str, s.m_str) == 0);
}

bool String::operator == (const char* s) const
{
    return (strcmp(m_str, s ? s : "") == 0);
}

bool String::operator != (const String& s) const
{
    return !(*this ==s);
}

bool String::operator != (const char* s) const
{
    return !(*this ==s);
}

bool String::operator > (const String& s) const
{
    return (strcmp(m_str, s.m_str) > 0);
}

bool String::operator > (const char* s) const
{
    return (strcmp(m_str, s ? s : "") > 0);
}

bool String::operator < (const String& s) const
{
    return (strcmp(m_str, s.m_str) < 0);
}

bool String::operator < (const char* s) const
{
    return (strcmp(m_str, s ? s : "") < 0);
}

bool String::operator >= (const String& s) const
{
    return (strcmp(m_str, s.m_str) >= 0);
}

bool String::operator >= (const char* s) const
{
    return (strcmp(m_str, s ? s : "") >= 0);
}

bool String::operator <= (const String& s) const
{
    return (strcmp(m_str, s.m_str) <= 0);
}

bool String::operator <= (const char* s) const
{
    return (strcmp(m_str, s) <= 0);
}

String String::operator +(const String& s) const
{
    return (*this + s.m_str);
}
String String::operator +(const char* s) const
{
    String ret;
    int len = m_length + static_cast<int>(strlen(s));
    char *str = reinterpret_cast<char*>(malloc(len +1));

    if(str)
    {
        strcpy(str, m_str);
        strcat(str, s);

        free(ret.m_str);

        ret.m_str = str;
        ret.m_length = len;
    }else {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memory to add string values...");
    }

    return ret;
}



String& String::operator = (const String& s)
{
    return (*this = s.m_str);
}
String& String::operator = (const char *s)
{
    if(m_str != s)
    {
        char *str = strdup(s);
        if(str)
        {
            free(m_str);
            m_str = str;
            m_length = static_cast<int>(strlen(m_str));
        }else {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to assign new String value...");
        }
    }

    return *this;
}
String& String::operator = (char c)
{
    char s[] = {c, '\0'};

    return (*this = s);
}

String String::operator +=(const String& s) const
{
    String ret = *this + s.m_str;

    return ret;
}

String String::operator +=(const char* s) const
{
    String ret = *this + s;
    return ret;
}



}
