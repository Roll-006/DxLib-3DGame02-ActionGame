#pragma once
#include "../Interface/i_tab.hpp"
#include "../Event/event_system.hpp"
#include "../UI/ui_selector.hpp"

#include "../UIButton/sub_menu_select_button.hpp"

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

	void Activate(const DeadPlayerEvent& event);

	[[nodiscard]] int	GetPriority()	const override { return m_priority; }
	[[nodiscard]] bool	IsActive()		const override { return m_is_active; }
	[[nodiscard]] bool	CanSelect()		const override { return m_can_select; }

private:
	void ExecuteContinue();
	void ExecuteQuitGame();

	void JudgeActive();

private:
	static constexpr Vector2D<int>	kFirstButtonCenterPos	= { Window::kCenterPos.x, Window::kCenterPos.y + 50 };
	static constexpr int			kButtonPosInterval		= 130;
	static constexpr float			kActiveWaitTime			= 6.0f;

	int	  m_priority;
	bool  m_is_active;
	bool  m_can_select;
	bool  m_can_calc_wait_time;
	float m_active_wait_timer;
	std::shared_ptr<UISelector> m_ui_selector;
};
