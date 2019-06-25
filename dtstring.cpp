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
}
