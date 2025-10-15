#pragma once
#include "../Interface/i_tab.hpp"
#include "../Event/event_system.hpp"

class GameOverTab final : public ITab
{
public:
	GameOverTab();
	~GameOverTab() override;

	void Init()			override;
	void Update()		override;
	void OnDraw() const override;

	[[nodiscard]] bool IsDraw() const override { return m_is_draw; }

private:
	bool m_is_draw;

	//std::shared_ptr<>
};
