#pragma once
#include "../Interface/i_tab.hpp"
#include "../Event/event_system.hpp"

class GameOverTab final
{
public:
	GameOverTab();
	~GameOverTab();

	void Init();
	void Update();

	[[nodiscard]] bool IsDraw() const { return m_is_draw; }

private:
	bool m_is_draw;
};
