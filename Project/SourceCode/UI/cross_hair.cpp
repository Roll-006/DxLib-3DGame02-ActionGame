#include "../Kind/player_state_kind.hpp"
#include "cross_hair.hpp"

CrossHair::CrossHair(std::shared_ptr<Player>& player) : 
	m_player					(player),
	m_current_hold_weapon_kind	(WeaponKind::kNone),
	m_current_hold_gun			(nullptr),
	m_current_hold_gun_kind		(GunKind::kHandgun),
	m_alpha_blend_num			(0),
	m_is_aiming					(false),
	m_circle_cross_hair			(std::make_shared<Circle>()),
	m_square_cross_hair			(std::make_shared<Square>()),
	m_cross_hair_screen			(std::make_shared<ScreenCreator>(kScreenSize, Window::kCenterPos))
{

}

CrossHair::~CrossHair()
{

}

void CrossHair::LateUpdate()
{
	const auto state_kind			= m_player->GetState()->GetCurrentStateKind();
	const auto is_aiming_gun		= state_kind == PlayerStateKind::kAimGun;
	const auto is_aiming_knife		= state_kind == PlayerStateKind::kAimKnife;
	const auto is_shot				= state_kind == PlayerStateKind::kShot;

	m_is_aiming					= is_aiming_gun || is_aiming_knife || is_shot;
	m_current_hold_weapon_kind	= m_player->GetCurrentHeldWeaponKind();

	switch (m_current_hold_weapon_kind)
	{
	case WeaponKind::kGun:
		CreateGunCrossHairScreen();
		break;

	case WeaponKind::kKnife:
		CreateKnifeCrossHairScreen();
		break;

	default:
		break;
	}	

	CalcAlphaBlendNum();
}

void CrossHair::Draw() const
{
	switch (m_current_hold_weapon_kind)
	{
	case WeaponKind::kGun:
		DrawGunCrossHair();
		break;

	case WeaponKind::kKnife:
		DrawKnifePoint();
		break;

	default:
		break;
	}
}


#pragma region Draw
void CrossHair::DrawKnifePoint() const
{
	m_cross_hair_screen->Draw();
}

void CrossHair::DrawGunCrossHair() const
{
	switch (m_current_hold_gun_kind)
	{
	case GunKind::kHandgun:
	case GunKind::kAssaultRifle:
	case GunKind::kRocketLauncher:
	case GunKind::kShotgun:
	case GunKind::kBoltThrower:
	case GunKind::kMagnum:
		m_cross_hair_screen->Draw();
		break;

	case GunKind::kSniperRifle:
		break;

	default:
		break;
	}
}
#pragma endregion


#pragma region スクリーン生成
void CrossHair::CreateKnifeCrossHairScreen()
{
	m_cross_hair_screen->UseScreen();
	DrawCircle(kScreenCenterSize.x, kScreenCenterSize.y, 2, 0xffffff, TRUE);
	m_cross_hair_screen->UnuseScreen();
}

void CrossHair::CreateGunCrossHairScreen()
{
	m_current_hold_gun		= std::static_pointer_cast<GunBase>(m_player->GetCurrentHeldWeapon());
	m_current_hold_gun_kind = m_current_hold_gun->GetGunKind();

	switch (m_current_hold_gun_kind)
	{
	case GunKind::kHandgun:
		break;

	case GunKind::kSniperRifle:
		break;

	case GunKind::kAssaultRifle:
		CreateSubmachineGunCrossHairScreen();
		break;

	case GunKind::kRocketLauncher:
		CreateRocketLauncherCrossScreen();
		break;

	case GunKind::kShotgun:
		break;

	case GunKind::kBoltThrower:
		break;

	case GunKind::kMagnum:
		break;
	}
}

void CrossHair::CreateSubmachineGunCrossHairScreen()
{
	CreateCircleCrossHair();

	const auto center_x	= kScreenCenterSize.x;
	const auto center_y = kScreenCenterSize.y;
	const auto radius	= static_cast<int>(m_circle_cross_hair->GetRadius());

	m_cross_hair_screen->UseScreen();
	DrawLine(center_x + radius, center_y,		   center_x + radius + kSubmachineGunWidth, center_y,								  0xffffff, kThickness);
	DrawLine(center_x - radius, center_y,		   center_x - radius - kSubmachineGunWidth, center_y,								  0xffffff, kThickness);
	DrawLine(center_x,			center_y + radius, center_x,								center_y + radius + kSubmachineGunHeight, 0xffffff, kThickness);
	m_cross_hair_screen->UnuseScreen();
}

void CrossHair::CreateRocketLauncherCrossScreen()
{
	CreateCircleCrossHair();

	const auto center_x		= kScreenCenterSize.x;
	const auto center_y		= kScreenCenterSize.y;
	const auto radius		= static_cast<int>(m_circle_cross_hair->GetRadius());
	const auto length		= 16;
	const auto half_length	= static_cast<int>(length * 0.5f);
	const auto left_pos		= center_x - radius - half_length;
	const auto right_pos	= center_x + radius + half_length;
	const auto up_pos		= center_y - radius - half_length;
	const auto down_pos		= center_y + radius + half_length;

	m_cross_hair_screen->UseScreen();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawCircle(center_x, center_y, radius, 0xffffff, FALSE, kThickness);
	DrawLine(left_pos, center_y, right_pos, center_y, 0xffffff, kThickness);
	DrawLine(center_x, up_pos,	 center_x,  down_pos, 0xffffff, kThickness);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawLine(left_pos,  center_y, left_pos  + length, center_y,			 0xffffff, kThickness);
	DrawLine(right_pos, center_y, right_pos - length, center_y,			 0xffffff, kThickness);
	DrawLine(center_x,  up_pos,   center_x,			  up_pos   + length, 0xffffff, kThickness);
	DrawLine(center_x,  down_pos, center_x,			  down_pos - length, 0xffffff, kThickness);

	m_cross_hair_screen->UnuseScreen();
}
#pragma endregion


void CrossHair::CreateCircleCrossHair()
{
	const auto circle = std::static_pointer_cast<Circle>(m_current_hold_gun->GetDiffusionShape());

	// 三次元空間の円を二次元空間に変換
	const auto point_on_circle_world	= circle->GetPos() + math::GetNormalVector(-circle->GetNormalVector(), axis::GetWorldYAxis()) * circle->GetRadius();
	const auto point_on_circle_screen	= ConvWorldPosToScreenPos(point_on_circle_world);
	const auto center_pos_screen		= ConvWorldPosToScreenPos(circle->GetPos());
	
	m_circle_cross_hair->SetPos(VECTOR(static_cast<float>(kScreenCenterSize.x), static_cast<float>(kScreenCenterSize.y), 0.0f));
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
		math::Increase(m_alpha_blend_num, static_cast<int>(kAlphaBlendIncreaseSpeed * delta_time), UCHAR_MAX, false);
	}
	else
	{
		math::Decrease(m_alpha_blend_num, static_cast<int>(kAlphaBlendDecreaseSpeed * delta_time), 0);
	}

	m_cross_hair_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);
}
