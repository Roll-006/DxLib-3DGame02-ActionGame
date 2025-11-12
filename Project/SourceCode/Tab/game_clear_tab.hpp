#pragma once
#include "../Interface/i_tab.hpp"
#include "../Event/event_system.hpp"
#include "../UI/ui_selector.hpp"

#include "../UIButton/sub_menu_select_button.hpp"
#include "../Part/scene_fader.hpp"

class GameClearTab final : public ITab
{
public:
	GameClearTab();
	~GameClearTab() override;

	void Init()			override;
	void Update()		override;
	void OnDraw(const int main_screen_handle) const override;

	void Activate() 	override { m_is_active = true; }
	void Deactivate()	override { m_is_active = false; }

	void AllowSelect()		override { m_can_select = true; }
	void DisallowSelect()	override { m_can_select = false; }


	[[nodiscard]] int  GetTabHandle()	const override	{ return m_tab_handle; }
	[[nodiscard]] int  GetPriority()	const override	{ return m_priority; }
	[[nodiscard]] bool IsActive()		const override	{ return m_is_active; }
	[[nodiscard]] bool CanSelect()		const override	{ return m_can_select; }
	[[nodiscard]] bool IsClear()		const			{ return m_is_clear && !SceneFader::GetInstance()->IsFading();; }

private:
	void StartActivateTimer(const DeadBossEvent& event);

	void JudgeActive();
	void JudgeClear();

	void CalcAlphaBlendNum();
	void CreateResultScreen();

private:
	static constexpr float kActiveWaitTime	= 4.0f;
	static constexpr float kFadeSpeed		= 300.0f;
	static constexpr float kClearWaitTime	= 10.0f;

	int	  m_tab_handle;
	int	  m_priority;
	bool  m_is_active;
	bool  m_can_select;

	float m_clear_wait_time;
	bool  m_is_clear;

	bool  m_can_calc_active_time;
	float m_active_timer;
	int	  m_alpha_blend_num;

	std::shared_ptr<UISelector>		m_ui_selector;
	std::shared_ptr<ScreenCreator>	m_result_screen;
};
