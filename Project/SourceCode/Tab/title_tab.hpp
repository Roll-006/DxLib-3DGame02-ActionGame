#pragma once
#include "../Interface/i_tab.hpp"

#include "../Event/event_system.hpp"
#include "../UI/ui_selector.hpp"
#include "../UIButton/main_menu_select_button.hpp"

class TitleTab final : public ITab
{
public:
	TitleTab();
	~TitleTab() override;

	void Init()			override;
	void Update()		override;
	void OnDraw(const int main_screen_handle) const override;

	void InitActivateWarningTab() { m_is_activate_warning_tab = false; }

	void Activate() 	override { m_is_active = true;  }
	void Deactivate()	override { m_is_active = false; }

	void AllowSelect()  override { m_can_select = true; }
	void StopSelect()	override { m_can_select = false; }

	[[nodiscard]] int  GetPriority()			const override	{ return m_priority; }
	[[nodiscard]] bool IsActive()				const override	{ return m_is_active; }
	[[nodiscard]] bool CanSelect()				const override	{ return m_can_select; }
	[[nodiscard]] bool IsGameStart()			const			{ return m_is_game_start; }
	[[nodiscard]] bool IsActivateWarningTab()	const			{ return m_is_activate_warning_tab; }

private:
	void ExecuteGameStart();
	void ExecuteExit();

private:
	static constexpr Vector2D<int>	kFirstButtonCenterPos	= { 390, 300 };
	static constexpr int			kButtonPosInterval		= 110;

	int	 m_priority;
	bool m_is_active;
	bool m_can_select;
	bool m_is_game_start;
	bool m_is_activate_warning_tab;
	std::shared_ptr<UISelector> m_ui_selector;
};
