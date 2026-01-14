#include "../../Object/player.hpp"
#include "../../Kind/player_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Kind/player_state_kind.hpp"
#include "player_state.hpp"
#include "player_attach_weapon.hpp"

player_state::AttachWeapon::AttachWeapon(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator) :
	PlayerStateBase(player, state, animator, PlayerStateKind::kAttachWeapon)
{
}

player_state::AttachWeapon::~AttachWeapon()
{
}

void player_state::AttachWeapon::Update()
{
	m_player.GetCurrentHeldWeapon()->Update();
}

void player_state::AttachWeapon::LateUpdate()
{
}

void player_state::AttachWeapon::Enter()
{
	m_player.DetachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kSub));
	m_player.HoldWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

void player_state::AttachWeapon::Exit()
{
	m_player.ReleaseWeapon();
	m_player.AttachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

const PlayerStateKind player_state::AttachWeapon::GetNextStateKind()
{
	//if (m_player.GetDeltaTime() <= 0.0f)
	//{
	//	return PlayerStateKind::kNone;
	//}

	//// 強制NULL
	//if (m_state.TryActionNullForcibly())
	//{
	//	return PlayerStateKind::kActionNull;
	//}

	return PlayerStateKind::kNone;
}
