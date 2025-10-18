#pragma once
#include <functional>
#include "../Interface/i_ui_element.hpp"

#include "../Command/command_handler.hpp"

class UIButtonBase abstract : public IUIElement
{
public:
	UIButtonBase(const std::function<void()> exeute_function);
	~UIButtonBase() override;

	void Activate() 	override { m_is_active = true;  }
	void Deactivate()	override { m_is_active = false; }

	void Exeute();

	[[nodiscard]] bool IsActive()	const override { return m_is_active; }

protected:
	bool					m_is_active;
	std::function<void()>	m_exeute_function;
};
