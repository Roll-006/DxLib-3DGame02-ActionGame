#pragma once
#include "../Object/player.hpp"
#include "../Part/screen_creator.hpp"
#include "../Font/font_handler.hpp"

class RemainingBulletsUI final
{
public:
	RemainingBulletsUI(std::shared_ptr<Player>& player);
	~RemainingBulletsUI();

	void LateUpdate();

	[[nodiscard]] int GetScreenHandle() const { return m_result_screen->GetScreenHandle(); }

private:
	void CreateResultScreen();
	void UpdateText();

private:
	static constexpr Vector2D<int>	kScreenSize		= { 256, 256 };
	static constexpr int			kTextOffset		= 6;
	static constexpr unsigned int	kZeroAmmoColor	= 0xc23a3a;
	static constexpr unsigned int	kAmmoColor		= 0xffffff;
	static constexpr unsigned int	kMaxAmmoColor	= 0x3ac23f;
	static constexpr unsigned int	kHaveAmmoColor	= 0xa1a1a1;
	static constexpr int			kLineWidth		= 6;
	static constexpr int			kLineThickness	= 2;

	std::shared_ptr<Player>			m_player;
	std::shared_ptr<ScreenCreator>	m_result_screen;	// 最終的な描画結果を描画するスクリーン

	int										m_remaining_bullets_font_handle;
	std::string								m_remaining_bullets_text;
	Vector2D<int>							m_remaining_bullets_font_size;
	unsigned int							m_remaining_bullets_font_color;

	int										m_have_ammo_font_handle;
	std::string								m_have_ammo_text;
	Vector2D<int>							m_have_ammo_font_size;
};
