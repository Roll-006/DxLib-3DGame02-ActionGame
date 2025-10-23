#pragma once
#include "../Object/player.hpp"

class CrossHair final
{
public:
	CrossHair(std::shared_ptr<Player>& player);
	~CrossHair();

	void LateUpdate();
	void Draw() const;

private:
	#pragma region Draw
	void DrawGunCrossHair() const;
	void DrawSubmachineGunCrossHair() const;
	#pragma endregion

	void CreateCircleCrossHair();
	void CreateSquareCrossHair();

	void CalcAlphaBlendNum();

private:
	static constexpr float	kAlphaBlendIncreaseSpeed = 1200.0f;
	static constexpr float	kAlphaBlendDecreaseSpeed = 800.0f;
	static constexpr int	kThickness				 = 2;
	static constexpr int	kSubmachineGunWidth		 = 40;
	static constexpr int	kSubmachineGunHeight	 = 20;

	std::shared_ptr<Player>&	m_player;

	WeaponKind					m_current_hold_weapon_kind;
	std::shared_ptr<GunBase>	m_current_hold_gun;
	GunKind						m_current_hold_gun_kind;
	int							m_alpha_blend_num;
	bool						m_is_aiming;

	std::shared_ptr<Circle>		m_circle_cross_hair;
	std::shared_ptr<Square>		m_square_cross_hair;
};
