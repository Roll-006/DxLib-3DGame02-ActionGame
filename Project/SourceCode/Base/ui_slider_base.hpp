#pragma once
#include <memory>

#include "../Interface/i_ui_element.hpp"
#include "../Part/gauge.hpp"
#include "../Command/command_handler.hpp"

class UISliderBase abstract : public IUIElement
{
public:
	UISliderBase(const float max_value, const float init_value, const float select_speed);
	~UISliderBase() override;

	void Activate() 	override { m_is_active = true;  }
	void Deactivate()	override { m_is_active = false; }

	void SetBasicGraphicer(const std::shared_ptr<Graphicer> basic_graphic) { m_basic_graphic = basic_graphic; }
	void SetThumbGraphicer(const std::shared_ptr<Graphicer> thumb_graphic) { m_thumb_graphic = thumb_graphic; }

	[[nodiscard]] bool IsActive() const override { return m_is_active; }

protected:
	void CalcGaugeValue();

protected:
	bool						m_is_active;
	float						m_select_speed;
	float						m_init_value;
	std::shared_ptr<Gauge>		m_gauge;
	std::shared_ptr<Graphicer>	m_basic_graphic;
	std::shared_ptr<Graphicer>	m_thumb_graphic;	// ‚Â‚Ü‚Ý•”•ª‚Ì‰æ‘œ
};
