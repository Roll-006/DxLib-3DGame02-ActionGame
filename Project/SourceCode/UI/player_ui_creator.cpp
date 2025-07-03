#include "player_ui_creator.hpp"

PlayerUICreator::PlayerUICreator(const std::shared_ptr<Player> player) : 
	m_player			(player),
	m_2d_diffusion_shape(nullptr)
{

}

PlayerUICreator::~PlayerUICreator()
{

}

void PlayerUICreator::LateUpdate()
{
	CreateCrossHair();
}

void PlayerUICreator::TestDraw()
{
	if (!m_2d_diffusion_shape) { return; }

	const auto shape = std::dynamic_pointer_cast<Circle>(m_2d_diffusion_shape);
	DrawCircle(shape->GetPos().x, shape->GetPos().y, shape->GetRadius(), 0xffffff, FALSE);
}

void PlayerUICreator::CreateCrossHair()
{
	const auto gun = std::dynamic_pointer_cast<GunBase>(m_player->GetCurrentAttachWeapon());

	if (gun == nullptr)		{ return; }
	if (!gun->IsAiming())	{ return; }

	// e‚Ìí—Ş‚É‚æ‚èƒNƒƒXƒwƒA‚ÌˆÊ’u‚ğ¶¬
	switch (gun->GetGunKind())
	{
	// MEMO : ”ÍˆÍ : ‰~, •`‰æ : \š
	case GunKind::kHandgun:
		break;

	// MEMO : ”ÍˆÍ : ƒJƒvƒZƒ‹, •`‰æ : ƒJƒvƒZƒ‹‚Ì”¼‰~•”•ª
	case GunKind::kShotgun:
		break;

	// MEMO : ”ÍˆÍ : ‰~, •`‰æ : \š(ã‚È‚µ)
	case GunKind::kSniperRifle:
		CreateSniperRifleCrossHair();
		break;

	case GunKind::kBoltThrower:
		break;

	// MEMO : ”ÍˆÍ : ³•ûŒ`, •`‰æ : \š(Î‚ß)
	case GunKind::kMagnum:
		break;

	// MEMO : ”ÍˆÍ : ‰~, •`‰æ : \š(ã‚È‚µ)
	case GunKind::kSubmachineGun:
		break;

	// MEMO : ”ÍˆÍ : ‰~, •`‰æ : ‰~ + \š
	case GunKind::kRocketLauncher:
		break;

	default:
		break;
	}
}

void PlayerUICreator::CreateSniperRifleCrossHair()
{
	const auto gun		= std::dynamic_pointer_cast<GunBase>(m_player->GetCurrentAttachWeapon());
	const auto circle	= std::dynamic_pointer_cast<Circle>(gun->GetDiffusionShape());

	const auto right_pos_3d = circle->GetPos() + math::GetNormalVector(-gun->GetAimDir(), axis::GetWorldYAxis()) * circle->GetRadius();
	const auto right_pos_2d = ConvWorldPosToScreenPos(right_pos_3d);
	const auto pos_2d		= ConvWorldPosToScreenPos(circle->GetPos());
	m_2d_diffusion_shape	= std::make_shared<Circle>(axis::GetWorldZAxis(), VSize(pos_2d - right_pos_2d));

	std::dynamic_pointer_cast<Circle>(m_2d_diffusion_shape)->SetPos(pos_2d);
}
