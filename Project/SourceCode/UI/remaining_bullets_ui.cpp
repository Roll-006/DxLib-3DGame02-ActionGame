#include "remaining_bullets_ui.hpp"

RemainingBulletsUI::RemainingBulletsUI(std::shared_ptr<Player>& player) : 
	m_player						(player),
	m_result_screen					(std::make_shared<ScreenCreator>(kScreenSize)),
	m_remaining_bullets_font_handle	(FontHandler::GetInstance()->GetFontHandle(FontName.REMAINING_BULLETS_TEXT)),
	m_remaining_bullets_text		(""),
	m_remaining_bullets_font_size	(v2d::GetZeroV<Vector2D<int>>()),
	m_remaining_bullets_font_color	(kAmmoColor),
	m_have_ammo_font_handle			(FontHandler::GetInstance()->GetFontHandle(FontName.HAVE_AMMO_NUM_TEXT)),
	m_have_ammo_text				(""),
	m_have_ammo_font_size			(v2d::GetZeroV<Vector2D<int>>())
{

}

RemainingBulletsUI::~RemainingBulletsUI()
{

}

void RemainingBulletsUI::LateUpdate()
{
	UpdateText();
	CreateResultScreen();
}

void RemainingBulletsUI::CreateResultScreen()
{
	m_result_screen->UseScreen();

	DrawStringToHandle(
		static_cast<int>((m_result_screen->GetScreenSize().x - m_remaining_bullets_font_size.x) * 0.5f - m_remaining_bullets_font_size.x * 0.5f - kTextOffset),
		static_cast<int>((m_result_screen->GetScreenSize().y - m_remaining_bullets_font_size.y) * 0.5f),
		m_remaining_bullets_text.c_str(), m_remaining_bullets_font_color, m_remaining_bullets_font_handle);

	DrawStringToHandle(
		static_cast<int>((m_result_screen->GetScreenSize().x - m_have_ammo_font_size.x) * 0.5f + m_have_ammo_font_size.x * 0.5f + kTextOffset),
		static_cast<int>((m_result_screen->GetScreenSize().y - m_have_ammo_font_size.y) * 0.5f),
		m_have_ammo_text.c_str(), 0xa1a1a1, m_have_ammo_font_handle);

	DrawLine(
		m_result_screen->GetHalfScreenSize().x + 2,
		static_cast<int>((m_result_screen->GetScreenSize().y - m_have_ammo_font_size.y) * 0.5f),
		m_result_screen->GetHalfScreenSize().x - 2,
		static_cast<int>((m_result_screen->GetScreenSize().y - m_have_ammo_font_size.y) * 0.5f + m_have_ammo_font_size.y),
		0xa1a1a1);

	m_result_screen->UnuseScreen();
}

void RemainingBulletsUI::UpdateText()
{
	const auto equip_weapon			= m_player->GetCurrentEquipWeapon(WeaponSlotKind::kMain);
	const auto equip_weapon_kind	= equip_weapon->GetWeaponKind();
	const auto ammo_holder			= m_player->GetAmmoHolder();

	// 銃装備時UI
	if (equip_weapon_kind == WeaponKind::kGun)
	{
		// 所持弾数を取得
		const auto gun			= std::static_pointer_cast<GunBase>(equip_weapon);
		const auto gun_kind		= gun->GetGunKind();
		const auto current_ammo	= gun->GetCurrentRemainingBulletNum();
		const auto max_ammo		= gun->GetMaxRemainingBulletNum();
		auto	   sum_ammo		= 0;

		switch (gun_kind)
		{
		case GunKind::kSubmachineGun:
			sum_ammo = ammo_holder->GetCurrentAmmoNum(typeid(AssaultRifleAmmoBox));
			break;

		case GunKind::kRocketLauncher:
			sum_ammo = ammo_holder->GetCurrentAmmoNum(typeid(RocketBombBox));
			break;

		default:
			break;
		}

		// テキストに変換
		m_remaining_bullets_text		= std::to_string(current_ammo);
		m_have_ammo_text				= std::to_string(sum_ammo);
		m_remaining_bullets_font_size	= { GetDrawStringWidthToHandle(m_remaining_bullets_text.c_str(), -1, m_remaining_bullets_font_handle), GetFontSizeToHandle(m_remaining_bullets_font_handle) };
		m_have_ammo_font_size			= { GetDrawStringWidthToHandle(m_have_ammo_text.c_str(),		 -1, m_have_ammo_font_handle),		   GetFontSizeToHandle(m_have_ammo_font_handle) };
		if (current_ammo >= max_ammo)
		{
			m_remaining_bullets_font_color = kMaxAmmoColor;
		}
		else if (current_ammo <= 0)
		{
			m_remaining_bullets_font_color = kZeroAmmoColor;
		}
		else
		{
			m_remaining_bullets_font_color = kAmmoColor;
		}
		
	}
}
