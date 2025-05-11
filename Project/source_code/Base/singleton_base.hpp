#pragma once

class SingletonBase
{
public:
	static void Generate()
	{
		if (!m_instance){ m_instance = new SingletonBase; }
	}

	static void Delete()
	{
		if (m_instance == nullptr)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}

	static SingletonBase* GetInstance() { return m_instance ? m_instance : nullptr; }

private:
	SingletonBase(){}
	virtual ~SingletonBase() = default;

private:
	static SingletonBase* m_instance;
};

SingletonBase* SingletonBase::m_instance = nullptr;
