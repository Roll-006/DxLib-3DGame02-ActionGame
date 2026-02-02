#pragma once

template<typename T>
class SingletonBase abstract
{
public:
    static void Generate()
    {
        if (!m_instance)
        {
            m_instance = new T;
        }
    }

    static void Delete()
    {
        if (m_instance != nullptr)
        {
            delete m_instance;
            m_instance = nullptr;
        }
    }

    [[nodiscard]] static T* GetInstance() { return m_instance ? m_instance : nullptr; }

protected:
    SingletonBase() {}
    virtual ~SingletonBase() = default;

private:
    static T* m_instance;
};

template<typename T>
T* SingletonBase<T>::m_instance = nullptr;
