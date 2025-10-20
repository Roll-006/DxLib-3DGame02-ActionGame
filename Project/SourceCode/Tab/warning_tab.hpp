#pragma once
#include "../Interface/i_tab.hpp"

#include "../Event/event_system.hpp"
#include "../UI/ui_selector.hpp"
#include "../UIButton/sub_menu_select_button.hpp"

class WarningTab final : public ITab
{
public:
	WarningTab();
	~WarningTab() override;

	void Init()			override;
	void Update()		override;
	void OnDraw(const int main_screen_handle) const override;

	void Activate() 	override { m_is_active = true; }
	void Deactivate()	override { m_is_active = false; }

	void AllowSelect()  override { m_can_select = true; }
	void StopSelect()	override { m_can_select = false; }

	[[nodiscard]] int  GetPriority()	const override	{ return m_priority; }
	[[nodiscard]] bool IsActive()		const override	{ return m_is_active; }
	[[nodiscard]] bool CanSelect()		const override	{ return m_can_select; }
	[[nodiscard]] bool IsBack()			const			{ return m_is_execute_back && m_result_screen->GetGraphicer()->GetAlphaBlendNum() <= 0; }

private:
	void ExecuteDecide();
	void ExecuteBack();

	void CalcAlphaBlendNum();
	void CreateResultScreen();

private:
	static constexpr Vector2D<int>	kFirstButtonCenterPos	= { Window::kCenterPos.x, Window::kCenterPos.y + 50 };
	static constexpr int			kButtonPosInterval		= 130;
	static constexpr float			kFadeSpeed				= 600.0f;

	int	 m_priority;
	bool m_is_active;
	bool m_can_select;
	bool m_is_execute_back;
	int  m_alpha_blend_num;
	std::shared_ptr<UISelector>		m_ui_selector;
	std::shared_ptr<Graphicer>		m_warning_icon_graphic;
	std::shared_ptr<ScreenCreator>	m_result_screen;

	int								m_font_handle;
	std::string						m_text;
	Vector2D<int>					m_font_size;
};
