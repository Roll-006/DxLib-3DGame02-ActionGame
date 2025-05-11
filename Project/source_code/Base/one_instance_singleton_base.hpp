#pragma once
#include <cassert>

/// @brief インスタンスの生成が一つに限られるシングルトンの基底クラス
class OneInstanceSingletonBase
{
public:
	OneInstanceSingletonBase()  { assert(!m_instantiated); m_instantiated = true; }
	~OneInstanceSingletonBase() { m_instantiated = false; }

private:
	static bool m_instantiated;
};
