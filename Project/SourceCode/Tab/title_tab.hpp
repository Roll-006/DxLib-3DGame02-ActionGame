#pragma once
#include "../Interface/i_tab.hpp"

#include "../Event/event_system.hpp"
#include "../UI/ui_selector.hpp"
#include "../UIButton/main_menu_select_button.hpp"
#include "../Part/scene_fader.hpp"

#include "tab_drawer.hpp"
#include "warning_tab.hpp"

class TitleTab final : public ITab
{
public:
	TitleTab();
	~TitleTab() override;

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
	[[nodiscard]] bool IsGameStart()	const			{ return m_is_game_start && !SceneFader::GetInstance()->IsFading(); }

private:
	void ExecuteGameStart();
	void ExecuteOption();
	void ExecuteExit();

private:
	static constexpr Vector2D<int>	kFirstButtonCenterPos	= { 440, 300 };
	static constexpr int			kButtonPosInterval		= 110;

	int								m_tab_handle;
	int								m_priority;
	bool							m_is_active;
	bool							m_can_select;
	bool							m_is_game_start;
	bool							m_is_exit;
	std::shared_ptr<UISelector>		m_ui_selector;
	std::shared_ptr<WarningTab>		m_warning_exit_tab;

	std::shared_ptr<ButtonPrompt>	m_button_prompt;
};
