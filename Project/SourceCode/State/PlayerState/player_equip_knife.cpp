#include "../../Object/player.hpp"
#include "../../Kind/player_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Kind/player_state_kind.hpp"
#include "player_state.hpp"
#include "player_equip_knife.hpp"

player_state::EquipKnife::EquipKnife(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator) :
	PlayerStateBase(player, state, animator, PlayerStateKind::kEquipKnife), m_elapsed_time(0.0f)
{
	m_basic_anim_kind				.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kEquipKnife;
	m_walk_forward_anim_kind		.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kEquipKnife;
	m_crouch_walk_forward_anim_kind	.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kEquipKnife;
	m_crouch_anim_kind				.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kEquipKnife;
}

player_state::EquipKnife::~EquipKnife()
{
}

void player_state::EquipKnife::Update()
{
	BasicMove();

	m_elapsed_time += GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kPlayer);
	m_player.GetCurrentHeldWeapon()->Update();
}

void player_state::EquipKnife::LateUpdate()
{

}

void player_state::EquipKnife::Enter()
{
	m_elapsed_time = 0.0f;

	m_player.DetachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kSub));
	m_player.HoldWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

void player_state::EquipKnife::Exit()
{
	m_player.ReleaseWeapon();
	m_player.AttachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

const PlayerStateKind player_state::EquipKnife::GetNextStateKind()
{
	if (m_player.GetDeltaTime() <= 0.0f)
	{
		return PlayerStateKind::kNone;
	}
	//// 強制 NULL
	//else if (m_state.TryActionNullForcibly())
	//{
	//	return PlayerStateKind::kIdle;
	//}
	
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
	// メレー(正面蹴り)
	else if (m_state.TryFrontKick())
	{
		return PlayerStateKind::kFrontKick;
	}
	// メレー(回し蹴り)
	else if (m_state.TryRoundhouseKick())
	{
		return PlayerStateKind::kRoundhouseKick;
	}
	// ステルスキル
	else if (m_state.TryStealthKill())
	{
		return PlayerStateKind::kStealthKill;
	}
	// 捕まれる
	else if (m_state.TryGrabbed())
	{
		return PlayerStateKind::kGrabbed;
	}

	// 回転斬り
	else if (m_state.TrySpinningSlash())
	{
		return PlayerStateKind::kSpinningSlashKnife;
	}
	// 横斬り（第一段階）
	else if (m_state.TryFirstSideSlashKnife())
	{
		return PlayerStateKind::kFirstSideSlashKnife;
	}
	// 銃装備（ショートカット）
	else if (m_state.TryEquipGunShortcut())
	{
		return PlayerStateKind::kEquipGun;
	}
	// 銃装備
	else if (m_state.TryEquipGun())
	{
		return PlayerStateKind::kEquipGun;
	}
	// 銃装備（強制）
	else if (m_elapsed_time > kReleaseKnifeForciblyTime && m_player.GetCurrentEquipWeaponKind(WeaponSlotKind::kMain) == WeaponKind::kGun)
	{
		return PlayerStateKind::kEquipGun;
	}
	// リロード
	else if (m_state.TryReload())
	{
		return PlayerStateKind::kReload;
	}
	// 強制解除
	else if (m_elapsed_time > kReleaseKnifeForciblyTime)
	{
		return PlayerStateKind::kIdle;
	}

	return PlayerStateKind::kNone;
}
