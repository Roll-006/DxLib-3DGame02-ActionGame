#include "player_ui_creator.hpp"

PlayerUICreator::PlayerUICreator(const std::shared_ptr<Player>& player) : 
	m_name				(UICreatorName.PLAYER_UI_CREATOR),
	m_priority			(10),
	m_is_active			(true),
	m_melee_target_icon	(std::make_shared<MeleeTargetIcon>(player->GetMeleeTarget())),
	m_escape_icon		(std::make_shared<EscapeIcon>(std::dynamic_pointer_cast<IGrabbable>(player))),
	m_status_icons		(std::make_shared<StatusIcons>(player)),
	m_weapon_shortcut	(std::make_shared<WeaponShortcutDrawer>(
							player->GetStateController()->GetWeaponActionState(),
							player->GetWeaponShortcutSelecter()))
{

}

PlayerUICreator::~PlayerUICreator()
{

}

void PlayerUICreator::LateUpdate()
{
	m_melee_target_icon	->LateUpdate();
	m_escape_icon		->LateUpdate();
	m_weapon_shortcut	->LateUpdate();
	m_status_icons		->LateUpdate();
}

void PlayerUICreator::OnDraw(const int main_screen_handle)
{
	m_melee_target_icon	->Draw();
	m_escape_icon		->Draw();
	m_weapon_shortcut	->Draw(main_screen_handle);
	m_status_icons		->Draw();

	//if (!m_2d_diffusion_shape) { return; }
	//if (!std::dynamic_pointer_cast<GunBase>(m_player->GetCurrentAttachWeapon())->IsAiming()) { return; }
	//
	//const auto shape = std::dynamic_pointer_cast<Circle>(m_2d_diffusion_shape);
	//
	//DrawCircle(
	//	static_cast<int>(shape->GetPos().x), 
	//	static_cast<int>(shape->GetPos().y), 
	//	static_cast<int>(shape->GetRadius()), 
	//	0xffffff, FALSE);
}

//void PlayerUICreator::CreateCrossHair()
//{
//	const auto gun = std::dynamic_pointer_cast<GunBase>(m_player->GetCurrentAttachWeapon());
//
//	if (gun == nullptr)		{ return; }
//	if (!gun->IsAiming())	{ return; }
//
//	// e‚Ìí—Ş‚É‚æ‚èƒNƒƒXƒwƒA‚ÌˆÊ’u‚ğ¶¬
//	switch (gun->GetGunKind())
//	{
//	// MEMO : ”ÍˆÍ : ‰~, •`‰æ : \š
//	case GunKind::kHandgun:
//		break;
//
//	// MEMO : ”ÍˆÍ : ƒJƒvƒZƒ‹, •`‰æ : ƒJƒvƒZƒ‹‚Ì”¼‰~•”•ª
//	case GunKind::kShotgun:
//		break;
//
//	// MEMO : ”ÍˆÍ : ‰~, •`‰æ : \š(ã‚È‚µ)
//	case GunKind::kSniperRifle:
//		CreateSniperRifleCrossHair();
//		break;
//
//	case GunKind::kBoltThrower:
//		break;
//
//	// MEMO : ”ÍˆÍ : ³•ûŒ`, •`‰æ : \š(Î‚ß)
//	case GunKind::kMagnum:
//		break;
//
//	// MEMO : ”ÍˆÍ : ‰~, •`‰æ : \š(ã‚È‚µ)
//	case GunKind::kSubmachineGun:
//		break;
//
//	// MEMO : ”ÍˆÍ : ‰~, •`‰æ : ‰~ + \š
//	case GunKind::kRocketLauncher:
//		break;
//
//	default:
//		break;
//	}
//}
//
//void PlayerUICreator::CreateSniperRifleCrossHair()
//{
//	const auto gun		= std::dynamic_pointer_cast<GunBase>(m_player->GetCurrentAttachWeapon());
//	const auto circle	= std::dynamic_pointer_cast<Circle> (gun->GetDiffusionShape());
//
//	// OŸŒ³‹óŠÔ‚Ì‰~‚ğ“ñŸŒ³‹óŠÔ‚É•ÏŠ·
//	const auto point_on_circle_world	= circle->GetPos() + math::GetNormalVector(-circle->GetNormalVector(), axis::GetWorldYAxis()) * circle->GetRadius();
//	const auto point_on_circle_screen	= ConvWorldPosToScreenPos(point_on_circle_world);
//	const auto center_pos_screen		= ConvWorldPosToScreenPos(circle->GetPos());
//	m_2d_diffusion_shape = std::make_shared<Circle>(axis::GetWorldZAxis(), VSize(center_pos_screen - point_on_circle_screen));
//
//	std::dynamic_pointer_cast<Circle>(m_2d_diffusion_shape)->SetPos(center_pos_screen);
//}
