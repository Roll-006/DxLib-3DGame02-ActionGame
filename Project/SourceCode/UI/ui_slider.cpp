#include "ui_slider.hpp"

UISlider::UISlider(const float max_value, const float init_value, const float select_speed) :
	m_is_active		(false),
	m_select_speed	(select_speed),
	m_gauge			(std::make_shared<Gauge>(max_value))
{
	m_gauge->SetCurrentValue(init_value);
}

UISlider::~UISlider()
{

}

void UISlider::Init()
{

}

void UISlider::Update()
{
	if (!m_is_active) { return; }

	const auto delta_time	= GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	const auto command		= CommandHandler ::GetInstance();
	auto value = m_gauge->GetCurrentValue();

	if (command->IsExecute(CommandKind::kSelectRight, TimeKind::kCurrent))
	{
		value += m_select_speed * delta_time;
		m_gauge->Increase(value);
	}
	if (command->IsExecute(CommandKind::kSelectLeft,  TimeKind::kCurrent))
	{
		value -= m_select_speed * delta_time;
		m_gauge->Decrease(value);
	}
}

void UISlider::Draw() const
{

}
