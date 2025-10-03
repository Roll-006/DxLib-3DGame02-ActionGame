#pragma once
#include "../Interface/i_observer.hpp"

template <typename T>
class Subject final
{
public:
	void AddObserver(std::shared_ptr<IObserver>& observer)
	{
		if (std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end())
		{
			m_observers.emplace_back(observer);
		}
	}

	void RemoveObserver(std::shared_ptr<IObserver>& observer)
	{
		if (std::find(m_observers.begin(), m_observers.end(), observer) != m_observers.end())
		{
			erase(m_observers, observer);
		}
	}

private:
	void Notify(const IEvent& event)
	{
		for (const auto& observer : m_observers)
		{
			observer->OnNotify(event);
		}
	}

private:
	std::vector<std::shared_ptr<IObserver>> m_observers;

	friend T;
};
