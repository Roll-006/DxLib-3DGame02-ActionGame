#pragma once
#include <cassert>

/// @brief インスタンスの生成が1つに制限する基底クラス
template<typename T>
class SingleInstanceBase abstract
{
public:
	SingleInstanceBase()
	{
		assert(m_instance == nullptr);
		m_instance = static_cast<T*>(this);
	}
	
	~SingleInstanceBase()
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
T* SingleInstanceBase<T>::m_instance = nullptr;
