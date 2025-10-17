#pragma once
#include "../Interface/i_tab.hpp"

#include "../Event/event_system.hpp"
#include "../Font/font_handler.hpp"
#include "../UI/ui_selector.hpp"
#include "../UIButton/select_button.hpp"

class TitleTab final : public ITab
{
public:
	TitleTab();
	~TitleTab() override;

	void Init()			override;
	void Update()		override;
	void OnDraw() const override;

	void Activate() 	override { m_is_active = true;  }
	void Deactivate()	override { m_is_active = false; }

	void AllowSelect()  override;
	void StopSelect()	override;

	[[nodiscard]] int  GetPriority()			const override	{ return m_priority; }
	[[nodiscard]] bool IsActive()				const override	{ return m_is_active; }
	[[nodiscard]] bool IsGameStart()			const			{ return m_is_game_start; }
	[[nodiscard]] bool IsActivateWarningTab()	const			{ return m_is_activate_warning_tab; }

private:
	void ExecuteGameStart();
	void ExecuteExit();

private:
	int	 m_priority;
	bool m_is_active;
	bool m_is_game_start;
	bool m_is_activate_warning_tab;
	std::shared_ptr<UISelector> m_ui_selector;
};
