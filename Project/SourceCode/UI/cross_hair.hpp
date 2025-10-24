#pragma once
#include "../Object/player.hpp"
#include "../Part/screen_creator.hpp"

class CrossHair final
{
public:
	CrossHair(std::shared_ptr<Player>& player);
	~CrossHair();

	void LateUpdate();
	void Draw() const;

private:
	void DrawKnifePoint() const;
	void DrawGunCrossHair() const;

	void CreateCircleCrossHair();
	void CreateSquareCrossHair();


	#pragma region ÉXÉNÉäÅ[Éìê∂ê¨
	void CreateKnifeCrossHairScreen();
	void CreateGunCrossHairScreen();
	void CreateSubmachineGunCrossHairScreen();
	void CreateRocketLauncherCrossScreen();
	#pragma endregion


	void CalcAlphaBlendNum();

private:
	static constexpr Vector2D<int>	kScreenSize				 = { 500, 500 };
	static constexpr Vector2D<int>	kScreenCenterSize		 = { static_cast<int>(kScreenSize.x * 0.5f), static_cast<int>(kScreenSize.y * 0.5f) };

	static constexpr float			kAlphaBlendIncreaseSpeed = 1200.0f;
	static constexpr float			kAlphaBlendDecreaseSpeed = 800.0f;
	static constexpr int			kThickness				 = 2;
	static constexpr int			kSubmachineGunWidth		 = 40;
	static constexpr int			kSubmachineGunHeight	 = 20;

private:
	std::shared_ptr<Player>&		m_player;

	WeaponKind						m_current_hold_weapon_kind;
	std::shared_ptr<GunBase>		m_current_hold_gun;
	GunKind							m_current_hold_gun_kind;
	int								m_alpha_blend_num;
	bool							m_is_aiming;

	std::shared_ptr<Circle>			m_circle_cross_hair;
	std::shared_ptr<Square>			m_square_cross_hair;
	std::shared_ptr<ScreenCreator>	m_cross_hair_screen;
};
