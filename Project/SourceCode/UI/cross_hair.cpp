#include "cross_hair.hpp"
#include "../Part/player_state_controller.hpp"

CrossHair::CrossHair(std::shared_ptr<Player>& player) : 
	m_player					(player),
	m_current_hold_weapon_kind	(WeaponKind::kNone),
	m_current_hold_gun			(nullptr),
	m_current_hold_gun_kind		(GunKind::kHandgun),
	m_alpha_blend_num			(0),
	m_is_aiming					(false),
	m_circle_cross_hair			(std::make_shared<Circle>()),
	m_square_cross_hair			(std::make_shared<Square>())
{

}

CrossHair::~CrossHair()
{

}

void CrossHair::LateUpdate()
{
	const auto weapon_action_state	= static_cast<player_state::WeaponActionStateKind>(m_player->GetStateController()->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind());
	const auto is_aiming_gun		= weapon_action_state == player_state::WeaponActionStateKind::kAimGun;
	const auto is_aiming_knife		= weapon_action_state == player_state::WeaponActionStateKind::kAimKnife;
	const auto is_shot				= weapon_action_state == player_state::WeaponActionStateKind::kShot;

	m_is_aiming					= is_aiming_gun || is_aiming_knife || is_shot;
	m_current_hold_weapon_kind	= m_player->GetCurrentHeldWeaponKind();

	if (m_current_hold_weapon_kind == WeaponKind::kGun)
	{
		m_current_hold_gun		= std::static_pointer_cast<GunBase>(m_player->GetCurrentHeldWeapon());
		m_current_hold_gun_kind = m_current_hold_gun->GetGunKind();

		switch (m_current_hold_gun_kind)
		{
		case GunKind::kHandgun:
		case GunKind::kSniperRifle:
		case GunKind::kSubmachineGun:
		case GunKind::kRocketLauncher:
			CreateCircleCrossHair();
			break;

		case GunKind::kShotgun:
		case GunKind::kBoltThrower:
		case GunKind::kMagnum:
			CreateSquareCrossHair();
			break;
		}
	}

	CalcAlphaBlendNum();
}

void CrossHair::Draw() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha_blend_num);

	switch (m_current_hold_weapon_kind)
	{
	case WeaponKind::kGun:
		DrawGunCrossHair();
		break;

	case WeaponKind::kKnife:
		DrawCircle(Window::kCenterPos.x, Window::kCenterPos.y, 2, 0xffffff, TRUE);
		break;

	default:
		break;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


#pragma region Draw
void CrossHair::DrawGunCrossHair() const
{
	switch (m_current_hold_gun_kind)
	{
	case GunKind::kHandgun:
		break;

	case GunKind::kSniperRifle:
		break;

	case GunKind::kSubmachineGun:
		DrawSubmachineGunCrossHair();
		break;

	case GunKind::kRocketLauncher:
		RocketLauncher();
		break;

	case GunKind::kShotgun:
		break;

	case GunKind::kBoltThrower:
		break;

	case GunKind::kMagnum:
		break;

	default:
		break;
	}
}

void CrossHair::DrawSubmachineGunCrossHair() const
{
	const auto center_x	= m_circle_cross_hair->GetPos().x;
	const auto center_y = m_circle_cross_hair->GetPos().y;
	const auto radius	= m_circle_cross_hair->GetRadius();

	DrawLine(center_x + radius, center_y,		   center_x + radius + kSubmachineGunWidth, center_y,								  0xffffff, kThickness);
	DrawLine(center_x - radius, center_y,		   center_x - radius - kSubmachineGunWidth, center_y,								  0xffffff, kThickness);
	DrawLine(center_x,			center_y + radius, center_x,								center_y + radius + kSubmachineGunHeight, 0xffffff, kThickness);
}

void CrossHair::RocketLauncher() const
{
	const auto center_x = m_circle_cross_hair->GetPos().x;
	const auto center_y = m_circle_cross_hair->GetPos().y;
	const auto radius	= m_circle_cross_hair->GetRadius();

	DrawCircle(center_x, center_y, radius, 0xffffff, FALSE, kThickness);
	DrawLine(center_x - radius, center_y, center_x + radius, center_y, 0xffffff, kThickness);
	DrawLine(center_x, center_y - radius, center_x, center_y + radius, 0xffffff, kThickness);
}
#pragma endregion


void CrossHair::CreateCircleCrossHair()
{
	const auto circle = std::static_pointer_cast<Circle>(m_current_hold_gun->GetDiffusionShape());

	// ŽOŽŸŒ³‹óŠÔ‚Ì‰~‚ð“ñŽŸŒ³‹óŠÔ‚É•ÏŠ·
	const auto point_on_circle_world	= circle->GetPos() + math::GetNormalVector(-circle->GetNormalVector(), axis::GetWorldYAxis()) * circle->GetRadius();
	const auto point_on_circle_screen	= ConvWorldPosToScreenPos(point_on_circle_world);
	const auto center_pos_screen		= ConvWorldPosToScreenPos(circle->GetPos());
	
	m_circle_cross_hair->SetPos(VECTOR(static_cast<float>(Window::kCenterPos.x), static_cast<float>(Window::kCenterPos.y), 0.0f));
	m_circle_cross_hair->SetRadius(VSize(center_pos_screen - point_on_circle_screen));
	m_circle_cross_hair->SetNormalVector(axis::GetWorldZAxis());
}

void CrossHair::CreateSquareCrossHair()
{

}

void CrossHair::CalcAlphaBlendNum()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	if (m_is_aiming)
	{
		math::Increase(m_alpha_blend_num, static_cast<int>(kAlphaBlendIncreaseSpeed * delta_time), 255, false);
	}
	else
	{
		math::Decrease(m_alpha_blend_num, static_cast<int>(kAlphaBlendDecreaseSpeed * delta_time), 0);
	}
}
