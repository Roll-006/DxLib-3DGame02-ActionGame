#pragma once
#include "../Interface/i_tab.hpp"
#include "../Event/event_system.hpp"
#include "../UI/ui_selector.hpp"

#include "../UIButton/sub_menu_select_button.hpp"
#include "../Part/scene_fader.hpp"
#include "../UI/game_clear_text.hpp"
#include "../UI/button_prompt.hpp"

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
	[[nodiscard]] bool IsRetry()		const { return m_is_retry	  && !SceneFader::GetInstance()->IsFading(); }
	[[nodiscard]] bool IsQuitGame()		const { return m_is_quit_game && !SceneFader::GetInstance()->IsFading(); }

private:
	void StartActivateTimer(const DeadAllEnemyEvent& event);

	void ExecuteRetry();
	void ExecuteQuitGame();

	void JudgeActive();

	void ChangeTimeScale();
	void CalcAlphaBlendNum();
	void CreateResultScreen();

private:
	static constexpr Vector2D<int>	kFirstButtonCenterPos	= { Window::kCenterPos.x, Window::kCenterPos.y + 80 };
	static constexpr int			kButtonPosInterval		= 130;
	static constexpr float			kActiveWaitTime			= 2.0f;
	static constexpr float			kFadeSpeed				= 300.0f;
	static constexpr float			kDrawResultWaitTime		= 10.0f;

	int		m_tab_handle;
	int		m_priority;
	bool	m_is_active;
	bool	m_can_select;
	bool	m_is_retry;
	bool	m_is_quit_game;

	bool	m_can_calc_active_time;
	float	m_active_timer;
	int		m_alpha_blend_num;

	float	m_change_time_scale_wait_time;
	bool	m_is_change_time_scale;

	std::shared_ptr<GameClearText>	m_game_clear_text;
	std::shared_ptr<UISelector>		m_ui_selector;
	std::shared_ptr<ScreenCreator>	m_result_screen;

	std::shared_ptr<ButtonPrompt>	m_button_prompt;
};
