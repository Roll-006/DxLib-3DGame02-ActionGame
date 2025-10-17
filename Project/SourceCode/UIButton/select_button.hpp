#pragma once
#include "../Base/ui_button_base.hpp"
#include "../Calculation/math.hpp"

class SelectButton final : public UIButtonBase
{
public:
	SelectButton(std::function<void()> exeute_function);
	~SelectButton() override;

	void Init()			override;
	void Update()		override;
	void Draw() const	override;

private:
	void CalcAlphaBlendNum();
	void CalcGraphicScale();

private:
	static constexpr int   kNonSelectAlphaBlendNum	= 100;
	static constexpr float kNonSelectScale			= 1.0f;
	static constexpr float kSelectScale				= 1.5f;
	static constexpr float kScaleChangeSpeed		= 10.0f;

	int	  m_alpha_blend_num;
	float m_current_graphic_scale;
	float m_destination_graphic_scale;
};
