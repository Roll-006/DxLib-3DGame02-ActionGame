#pragma once
#include "../Interface/i_tab.hpp"

#include "../Event/event_system.hpp"
#include "../UI/ui_selector.hpp"
#include "../UIButton/main_menu_select_button.hpp"
#include "../Path/ui_graphic_path.hpp"
#include "../Part/scene_fader.hpp"

#include "tab_drawer.hpp"
#include "warning_tab.hpp"

class PauseTab final : public ITab
{
public:
	PauseTab();
	~PauseTab() override;

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
	[[nodiscard]] bool IsReturnToGame() const			{ return m_is_execute_return_to_game && m_result_screen->GetGraphicer()->GetBlendNum() <= 0;; }
	[[nodiscard]] bool IsRestart()		const			{ return m_is_restart && !SceneFader::GetInstance()->IsFading(); }
	[[nodiscard]] bool IsOption()		const			{ return m_is_option; }
	[[nodiscard]] bool IsQuitGame()		const			{ return m_is_quit_game && !SceneFader::GetInstance()->IsFading(); }

private:
	void Deactivate(const DeadPlayerEvent&	event);
	void Deactivate(const DeadAllEnemyEvent&	event);

	void ExecuteReturnToGame();
	void ExecuteRestart();
	void ExecuteOption();
	void ExecuteQuitGame();

	void JudgeActive();
	void JudgeDeactivate();
	void JudgeSelect();

	void CalcAlphaBlendNum();
	void CreateResultScreen();

	/// @brief タブを閉じる
	/// @brief UIボタンからではなくタブを開くコマンドから閉じる
	void BackTab();

private:
	static constexpr Vector2D<int>	kFirstButtonCenterPos	= { 440, 300 };
	static constexpr int			kButtonPosInterval		= 110;
	static constexpr float			kFadeSpeed = 600.0f;

	int												m_tab_handle;
	int												m_priority;
	bool											m_is_active;
	bool											m_is_deactivate_forcibly;
	bool											m_can_select;
	bool											m_is_execute_return_to_game;
	bool											m_is_restart;
	bool											m_is_option;
	bool											m_is_quit_game;
	int												m_alpha_blend_num;
	std::shared_ptr<UISelector>						m_ui_selector;
	std::shared_ptr<ScreenCreator>					m_result_screen;

	std::unordered_map<TimeScaleLayerKind, float>	m_prev_time_scale;
	std::shared_ptr<WarningTab>						m_warning_restart_tab;
	std::shared_ptr<WarningTab>						m_warning_quit_game_tab;

	std::shared_ptr<ButtonPrompt>					m_button_prompt;
};
