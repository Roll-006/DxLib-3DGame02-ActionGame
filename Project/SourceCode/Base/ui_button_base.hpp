#pragma once
#include <functional>
#include "../Interface/i_ui_element.hpp"

#include "../Command/command_handler.hpp"

class UIButtonBase abstract : public IUIElement
{
public:
	UIButtonBase(std::function<void()> exeute_function);
	~UIButtonBase() override;

	void Activate() 	override { m_is_active = true;  }
	void Deactivate()	override { m_is_active = false; }

	void Exeute();

	void SetGraphicer(const std::shared_ptr<Graphicer> graphic) { m_graphic = graphic; }

	[[nodiscard]] bool IsActive() const override { return m_is_active; }

protected:
	bool m_is_active;
	std::shared_ptr<Graphicer>	m_graphic;
	std::function<void()>		m_exeute_function;
};
