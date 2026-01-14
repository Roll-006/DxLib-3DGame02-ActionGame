#include "../../Object/player.hpp"
#include "../../Kind/player_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Event/event_system.hpp"
#include "../../Kind/player_state_kind.hpp"
#include "../../Command/command_handler.hpp"
#include "player_state.hpp"
#include "player_reload.hpp"

player_state::Reload::Reload(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator) :
	PlayerStateBase			(player, state, animator, PlayerStateKind::kReload),
	m_is_release_ammo_box	(false),
	m_is_set_ammo_box		(false),
	m_is_cocking			(false)
{
	m_basic_anim_kind				.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kReload;
	m_walk_forward_anim_kind		.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kReload;
	m_run_forward_anim_kind			.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kReload;
	m_crouch_walk_forward_anim_kind	.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kReload;
	m_crouch_anim_kind				.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kReload;
}

player_state::Reload::~Reload()
{
}

void player_state::Reload::Update()
{
	BasicMove();

	const auto gun			= std::static_pointer_cast<GunBase>(m_player.GetCurrentHeldWeapon());
	const auto play_rate	= m_animator->GetPlayRate(Animator::BodyKind::kUpperBody);
	const auto event_system = EventSystem::GetInstance();

	m_player.GetCurrentHeldWeapon()->Update();

	// 弾倉を外す
	if (play_rate > 0.0f && !m_is_release_ammo_box)
	{
		event_system->Publish(ReleaseAmmoBoxEvent(gun->GetTransform()->GetPos(CoordinateKind::kWorld), TimeScaleLayerKind::kPlayer));
		m_is_release_ammo_box = true;
	}

	// 弾倉をセット
	if (play_rate > 0.5f && !m_is_set_ammo_box)
	{
		event_system->Publish(SetAmmoBoxEvent(gun->GetTransform()->GetPos(CoordinateKind::kWorld), TimeScaleLayerKind::kPlayer));

		gun->GetMagazine()->OnReloaded();
		m_player.GetAmmoHolder()->Reload(gun);
		m_is_set_ammo_box = true;
	}

	// コッキング
	if (play_rate > 0.7f && !m_is_cocking)
	{
		event_system->Publish(CockingEvent(gun->GetTransform()->GetPos(CoordinateKind::kWorld), TimeScaleLayerKind::kPlayer));
		m_is_cocking = true;
	}
}

void player_state::Reload::LateUpdate()
{

}

void player_state::Reload::Enter()
{
	m_is_release_ammo_box = false;
	m_is_set_ammo_box = false;
	m_is_cocking = false;

	m_player.DetachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));
	m_player.HoldWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));

	const auto gun = std::static_pointer_cast<GunBase>(m_player.GetCurrentHeldWeapon());
	gun->GetMagazine()->OnStartReload(m_player.GetModeler());
}

void player_state::Reload::Exit()
{
	m_player.ReleaseWeapon();
	m_player.AttachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));
}

const PlayerStateKind player_state::Reload::GetNextStateKind()
{
	const auto command = CommandHandler::GetInstance();

	if (m_player.GetDeltaTime() <= 0.0f)
	{
		return PlayerStateKind::kNone;
	}

	// 勝利ポーズ
	else if (m_player.IsVictoryPose())
	{
		return PlayerStateKind::kVictoryPose;
	}
	// 死亡
	else if (m_state.TryDead())
	{
		return PlayerStateKind::kDead;
	}
	// 捕まれる
	else if (m_state.TryGrabbed())
	{
		return PlayerStateKind::kGrabbed;
	}

	// 銃エイミング状態
	else if (m_player.CanControl() && m_animator->IsPlayEnd(Animator::BodyKind::kUpperBody) && command->IsExecute(CommandKind::kAimGun, TimeKind::kCurrent))
	{
		return PlayerStateKind::kAimGun;
	}
	// 銃装備状態
	else if (m_animator->IsPlayEnd(Animator::BodyKind::kUpperBody))
	{
		return PlayerStateKind::kEquipGun;
	}

	return PlayerStateKind::kNone;
}