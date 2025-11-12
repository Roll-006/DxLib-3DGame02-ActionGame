#pragma once
#include "../Part/screen_creator.hpp"
#include "../Font/font_handler.hpp"
#include "../Calculation/math.hpp"

class GameClearText final
{
public:
	GameClearText();
	~GameClearText();

	void Init();
	void Update();
	void Draw() const;

private:
	void CreateResultScreen();

private:
	static constexpr Vector2D<int> kScreenSize	= { 1500, 300 };
	static constexpr Vector2D<int> kCenterPos	= { Window::kCenterPos.x, static_cast<int>(Window::kCenterPos.y * 0.7f ) };

	int								m_font_handle;
	std::string						m_text;
	Vector2D<int>					m_font_size;

	int								m_alpha_blend_num;
	float							m_scale;
	std::shared_ptr<ScreenCreator>	m_result_screen;
};
