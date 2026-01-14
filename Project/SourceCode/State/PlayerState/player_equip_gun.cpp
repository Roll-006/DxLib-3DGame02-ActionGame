#include "../../Object/player.hpp"
#include "../../Kind/player_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Event/event_system.hpp"
#include "../../Kind/player_state_kind.hpp"
#include "../../Command/command_handler.hpp"
#include "player_state.hpp"
#include "player_equip_gun.hpp"

player_state::EquipGun::EquipGun(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator) :
	PlayerStateBase(player, state, animator, PlayerStateKind::kEquipGun),
	m_possible_aim_timer(0.0f)
{
	m_basic_anim_kind				.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kEquipGun;
	m_walk_forward_anim_kind		.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kEquipGun;
	m_run_forward_anim_kind			.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kEquipGun;
	m_crouch_walk_forward_anim_kind	.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kEquipGun;
	m_crouch_anim_kind				.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kEquipGun;
}

player_state::EquipGun::~EquipGun()
{
}

void player_state::EquipGun::Update()
{
	BasicMove();

	if (CommandHandler::GetInstance()->IsExecute(CommandKind::kAimGun, TimeKind::kCurrent))
	{
		m_possible_aim_timer += GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kPlayer);
	}
	else
	{
		m_possible_aim_timer = 0.0f;
	}

	m_player.GetCurrentHeldWeapon()->Update();
}

void player_state::EquipGun::LateUpdate()
{
}

void player_state::EquipGun::Enter()
{
	m_possible_aim_timer = 0.0f;

	m_player.DetachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));
	m_player.HoldWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));

	// 直前がエイムなら解除通知
	const auto prev = m_state.GetPrevStateKind();
	if (prev == PlayerStateKind::kAimGun)
	{
		const auto gun = std::static_pointer_cast<GunBase>(m_player.GetCurrentHeldWeapon());
		EventSystem::GetInstance()->Publish(
			ExitAimGunEvent(gun->GetTransform()->GetPos(CoordinateKind::kWorld), TimeScaleLayerKind::kPlayer)
		);
	}
}

void player_state::EquipGun::Exit()
{
	m_player.ReleaseWeapon();
	m_player.AttachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));
}

const PlayerStateKind player_state::EquipGun::GetNextStateKind()
{
	if (m_player.GetDeltaTime() <= 0.0f)
	{
		return PlayerStateKind::kNone;
	}
	//// 強制 NULL
	//else if (m_state.())
	//{
	//	return PlayerStateKind::kActionNull;
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

	// 銃エイム
	else if (m_player.CanControl()
		&& CommandHandler::GetInstance()->IsExecute(CommandKind::kAimGun, TimeKind::kCurrent)
		&& m_possible_aim_timer >= kPossibleAimTime)
	{
		return PlayerStateKind::kAimGun;
	}
	// リロード
	else if (m_state.TryReload())
	{
		return PlayerStateKind::kReload;
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

	return PlayerStateKind::kNone;
}
