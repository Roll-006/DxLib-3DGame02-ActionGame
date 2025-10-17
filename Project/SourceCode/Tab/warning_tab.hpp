#pragma once
#include "../Interface/i_tab.hpp"

#include "../Event/event_system.hpp"
#include "../Font/font_handler.hpp"
#include "../UI/ui_selector.hpp"
#include "../UIButton/select_button.hpp"

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

	[[nodiscard]] int  GetPriority()	const override	{ return m_priority; }
	[[nodiscard]] bool IsActive()		const override	{ return m_is_active; }
	[[nodiscard]] bool IsBack()			const			{ return m_back; }

private:
	void ExecuteDecide();
	void ExecuteBack();

private:
	int	 m_priority;
	bool m_is_active;
	bool m_back;
	std::shared_ptr<UISelector> m_ui_selector;
};
