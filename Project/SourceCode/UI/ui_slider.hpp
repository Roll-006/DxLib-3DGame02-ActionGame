#pragma once
#include <memory>

#include "../Interface/i_ui_element.hpp"
#include "../Part/gauge.hpp"
#include "../Command/command_handler.hpp"

class UISlider final : public IUIElement
{
public:
	UISlider(const float max_value, const float init_value, const float select_speed);
	~UISlider() override;

	void Init()			override; 
	void Update()		override;
	void Draw() const	override;

	void Activate() 	override { m_is_active = true;  }
	void Deactivate()	override { m_is_active = false; }

	[[nodiscard]] bool IsActive() const override { return m_is_active; }

private:
	bool					m_is_active;
	float					m_select_speed;
	float					m_init_value;
	std::shared_ptr<Gauge>	m_gauge;
};
