#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

namespace DTLib
{
template <typename T>
class SmartPointer
{
protected:
    T * m_pointer;
public:
    SmartPointer(T *p = nullptr)
    {
        m_pointer = p;
    }

    SmartPointer(const SmartPointer<T>& obj)
    {
        m_pointer = obj.m_pointer;
        // obj.m_pointer = nullptr;
        const_cast<SmartPointer<T>&>(obj).m_pointer = nullptr;
    }

    SmartPointer<T>& operator= (const SmartPointer<T>& obj)
    {
        if(this != &obj)
        {
            delete m_pointer;

            obj.m_pointer = nullptr;
            const_cast<SmartPointer<T>&>(obj).m_pointer = nullptr;

        }

        return *this;
    }

    T* operator-> ()
    {
        return m_pointer;
    }

    T& operator* ()
    {
        return *m_pointer;
    }

    bool isNull()
    {
        return (m_pointer == nullptr);
    }

    T* get()
    {
        return m_pointer;
    }

    ~SmartPointer()
    {
        delete m_pointer;
    }
};
}

#endif // SMARTPOINTER_H
