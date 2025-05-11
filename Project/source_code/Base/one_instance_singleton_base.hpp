#pragma once
#include <cassert>

/// @brief インスタンスの生成が1つに限られるシングルトンの基底クラス
template<typename T>
class OneInstanceSingletonBase
{
public:
	OneInstanceSingletonBase()
	{
		assert(m_instance == nullptr);
		m_instance = static_cast<T*>(this);
	}
	
	~OneInstanceSingletonBase()
	{
		if (m_instance != nullptr)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}

private:
	static T* m_instance;
};

template<typename T>
T* OneInstanceSingletonBase<T>::m_instance = nullptr;
