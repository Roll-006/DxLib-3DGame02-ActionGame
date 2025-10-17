#include "select_button.hpp"

SelectButton::SelectButton(std::function<void()> exeute_function) :
	UIButtonBase				(exeute_function),
	m_alpha_blend_num			(kNonSelectAlphaBlendNum),
	m_current_graphic_scale		(kNonSelectScale),
	m_destination_graphic_scale	(kNonSelectScale)
{

}

SelectButton::~SelectButton()
{

}

void SelectButton::Init()
{

}

void SelectButton::Update()
{
	CalcAlphaBlendNum();
	CalcGraphicScale();
	Exeute();
}

void SelectButton::Draw() const
{
	if (m_graphic) { m_graphic->Draw(); }
}

void SelectButton::CalcAlphaBlendNum()
{
	if (!m_graphic) { return; }

	m_alpha_blend_num = m_is_active ? 255 : kNonSelectAlphaBlendNum;
	m_graphic->SetAlphaBlendNum(m_alpha_blend_num);
}

void SelectButton::CalcGraphicScale()
{
	if (!m_graphic) { return; }

	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	if (m_is_active)
	{
		m_destination_graphic_scale = kSelectScale;
		math::Increase(m_current_graphic_scale, kScaleChangeSpeed * delta_time, m_destination_graphic_scale, false);
	}
	else
	{
		m_destination_graphic_scale = kNonSelectScale;
		math::Decrease(m_current_graphic_scale, kScaleChangeSpeed * delta_time, m_destination_graphic_scale);
	}

	m_graphic->SetScale(m_current_graphic_scale);
}
