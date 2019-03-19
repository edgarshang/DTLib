#ifndef EXCEPTION_H
#define EXCEPTION_H

namespace DTLib
{

#define THROW_EXCEPTION(e, m) (throw e(m, __FILE__, __LINE__))
class Exception
{
protected:
    char* m_message;
    char* m_location;

    void init(const char* message, const char* file, int line);
public:
    Exception(const char* message);
    Exception(const char* file, int line);
    Exception(const char* message, const char* file, int len);

    Exception(const Exception& e);
    Exception& operator= (const Exception& e);

    virtual const char* message()const;
    virtual const char* location() const;

    virtual ~Exception() = 0;
};

class ArithmeticException : public Exception
{
public:
    ArithmeticException() : Exception(nullptr, nullptr, 0){}
    ArithmeticException(const char* message):Exception(message){}
    ArithmeticException(const char* file, int line) : Exception(file, line){}
    ArithmeticException(const char* message, const char* file, int line) : Exception(message, file, line){}

    ArithmeticException(const ArithmeticException& e) : Exception(e){}
    ArithmeticException& operator= (const ArithmeticException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

class NullPointerException : public Exception
{
public:
    NullPointerException() : Exception(nullptr, nullptr, 0){}
    NullPointerException(const char* message):Exception(message){}
    NullPointerException(const char* file, int line) : Exception(file, line){}
    NullPointerException(const char* message, const char* file, int line) : Exception(message, file, line){}

    NullPointerException(const NullPointerException& e) : Exception(e){}
    NullPointerException& operator= (const NullPointerException& e)
    {
        Exception::operator=(e);

        return *this;
    }

};

class IndexOutOfBoundException : public Exception
{
public:
    IndexOutOfBoundException() : Exception(nullptr, nullptr, 0){}
    IndexOutOfBoundException(const char* message):Exception(message){}
    IndexOutOfBoundException(const char* file, int line) : Exception(file, line){}
    IndexOutOfBoundException(const char* message, const char* file, int line) : Exception(message, file, line){}

    IndexOutOfBoundException(const IndexOutOfBoundException& e) : Exception(e){}
    IndexOutOfBoundException& operator= (const IndexOutOfBoundException& e)
    {
        Exception::operator=(e);

        return *this;
    }

};

class NoEnoughMemory : public Exception
{
public:
    NoEnoughMemory() : Exception(nullptr, nullptr, 0){}
    NoEnoughMemory(const char* message):Exception(message){}
    NoEnoughMemory(const char* file, int line) : Exception(file, line){}
    NoEnoughMemory(const char* message, const char* file, int line) : Exception(message, file, line){}

    NoEnoughMemory(const NoEnoughMemory& e) : Exception(e){}
    NoEnoughMemory& operator= (const NoEnoughMemory& e)
    {
        Exception::operator=(e);

        return *this;
    }

};

class InvalidParameterException : public Exception
{
public:
    InvalidParameterException() : Exception(nullptr, nullptr, 0){}
    InvalidParameterException(const char* message):Exception(message){}
    InvalidParameterException(const char* file, int line) : Exception(file, line){}
    InvalidParameterException(const char* message, const char* file, int line) : Exception(message, file, line){}

    InvalidParameterException(const InvalidParameterException& e) : Exception(e){}
    InvalidParameterException& operator= (const InvalidParameterException& e)
    {
        Exception::operator=(e);

        return *this;
    }

};
}


#endif // EXCEPTION_H
