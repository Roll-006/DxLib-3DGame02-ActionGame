#pragma once
#include "../Interface/i_tab.hpp"
#include "../Event/event_system.hpp"
#include "../UI/ui_selector.hpp"

class GameOverTab final : public ITab
{
public:
	GameOverTab();
	~GameOverTab() override;

	void Init()			override;
	void Update()		override;
	void OnDraw() const override;

	void Activate() 	override { m_is_active = true;  }
	void Deactivate()	override { m_is_active = false; }

	void AllowSelect()  override { m_can_select = true; }
	void StopSelect()	override { m_can_select = false; }

	[[nodiscard]] int	GetPriority()	const override { return m_priority; }
	[[nodiscard]] bool	IsActive()		const override { return m_is_active; }
	[[nodiscard]] bool	CanSelect()		const override { return m_can_select; }

private:
	int	 m_priority;
	bool m_is_active;
	bool m_can_select;
	std::shared_ptr<UISelector> m_ui_selector;
};
