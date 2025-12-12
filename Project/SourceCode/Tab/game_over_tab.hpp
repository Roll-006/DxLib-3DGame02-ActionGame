#pragma once
#include "../Interface/i_tab.hpp"
#include "../Event/event_system.hpp"
#include "../UI/ui_selector.hpp"

#include "../UIButton/sub_menu_select_button.hpp"
#include "../Part/scene_fader.hpp"
#include "../UI/game_over_text.hpp"
#include "../UI/button_prompt.hpp"

class GameOverTab final : public ITab
{
public:
	GameOverTab();
	~GameOverTab() override;

	void Init()			override;
	void Update()		override;
	void OnDraw(const int main_screen_handle) const override;

	void Activate() 	override { m_is_active = true;  }
	void Deactivate()	override { m_is_active = false; }

	void AllowSelect()		override { m_can_select = true; }
	void DisallowSelect()	override { m_can_select = false; }


	[[nodiscard]] int  GetTabHandle()	const override	{ return m_tab_handle; }
	[[nodiscard]] int  GetPriority()	const override	{ return m_priority; }
	[[nodiscard]] bool IsActive()		const override	{ return m_is_active; }
	[[nodiscard]] bool CanSelect()		const override	{ return m_can_select; }
	[[nodiscard]] bool IsContinue()		const			{ return m_is_continue  && !SceneFader::GetInstance()->IsFading(); }
	[[nodiscard]] bool IsQuitGame()		const			{ return m_is_quit_game && !SceneFader::GetInstance()->IsFading(); }

private:
	void Activate(const DeadPlayerEvent& event);

	void ExecuteContinue();
	void ExecuteQuitGame();

	void JudgeActive();
	void CalcAlphaBlendNum();
	void CreateResultScreen();

private:
	static constexpr Vector2D<int>	kFirstButtonCenterPos	= { Window::kCenterPos.x, Window::kCenterPos.y + 50 };
	static constexpr int			kButtonPosInterval		= 130;
	static constexpr float			kActiveWaitTime			= 6.0f;
	static constexpr float			kFadeSpeed				= 400.0f;

	int								m_tab_handle;
	int								m_priority;
	bool							m_is_active;

	bool							m_can_select;
	bool							m_can_calc_wait_time;
	bool							m_is_continue;
	bool							m_is_quit_game;
	float							m_active_wait_timer;
	int								m_alpha_blend_num;

	std::shared_ptr<GameOverText>	m_game_over_text;
	std::shared_ptr<UISelector>		m_ui_selector;
	std::shared_ptr<Graphicer>		m_filter_graphic;
	std::shared_ptr<ScreenCreator>	m_result_screen;

	std::shared_ptr<ButtonPrompt>	m_button_prompt;
};
