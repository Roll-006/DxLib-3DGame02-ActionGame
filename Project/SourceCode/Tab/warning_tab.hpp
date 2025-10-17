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
	void OnDraw() const override;

	void Activate() 	override { m_is_active = true; }
	void Deactivate()	override { m_is_active = false; }

	void AllowSelect()  override { m_can_select = true; }
	void StopSelect()	override { m_can_select = false; }

	[[nodiscard]] int  GetPriority()	const override	{ return m_priority; }
	[[nodiscard]] bool IsActive()		const override	{ return m_is_active; }
	[[nodiscard]] bool CanSelect()		const override	{ return m_can_select; }
	[[nodiscard]] bool IsBack()			const			{ return m_back; }

private:
	void ExecuteDecide();
	void ExecuteBack();

private:
	static constexpr Vector2D<int>	kFirstButtonCenterPos	= { Window::kCenterPos.x, Window::kCenterPos.y + 50 };
	static constexpr int			kButtonPosInterval		= 80;

	int	 m_priority;
	bool m_is_active;
	bool m_can_select;
	bool m_back;
	std::shared_ptr<UISelector> m_ui_selector;
};
