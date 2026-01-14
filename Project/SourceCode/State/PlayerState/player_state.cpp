#include <memory>
#include <unordered_map>
#include "../../Object/player.hpp"
#include "../../Animator/animator.hpp"
#include "../../Base/player_state_base.hpp"
#include "../../Kind/player_state_kind.hpp"
#include "../../Command/command_handler.hpp"

#include "player_idle.hpp"
#include "player_dead.hpp"
#include "player_attach_weapon.hpp"
#include "player_detach_weapon.hpp"
#include "player_aim_gun.hpp"
#include "player_equip_gun.hpp"
#include "player_equip_knife.hpp"
#include "player_reload.hpp"
#include "player_shot.hpp"
#include "player_shot_rocket_launcher.hpp"
#include "player_first_side_slash_knife.hpp"
#include "player_second_side_slash_knife.hpp"
#include "player_spinning_slash_knife.hpp"
#include "player_front_kick.hpp"
#include "player_roundhouse_kick.hpp"
#include "player_escape.hpp"
#include "player_grabbed.hpp"
#include "player_stealth_kill.hpp"
#include "player_victory_pose.hpp"

#include "player_state.hpp"

player_state::State::State(Player& player, const std::shared_ptr<Animator>& animator) :
	m_player			(player),
	m_current_state		(nullptr),
	m_prev_state_kind	(PlayerStateKind::kIdle)
{
	m_states[PlayerStateKind::kIdle]					= std::make_shared<player_state::Idle>					(m_player, *this, animator);
	m_states[PlayerStateKind::kDead]					= std::make_shared<player_state::Dead>					(m_player, *this, animator);
	m_states[PlayerStateKind::kAttachWeapon]			= std::make_shared<player_state::AttachWeapon>			(m_player, *this, animator);
	m_states[PlayerStateKind::kDetachWeapon]			= std::make_shared<player_state::DetachWeapon>			(m_player, *this, animator);
	m_states[PlayerStateKind::kAimGun]					= std::make_shared<player_state::AimGun>				(m_player, *this, animator);
	m_states[PlayerStateKind::kEquipGun]				= std::make_shared<player_state::EquipGun>				(m_player, *this, animator);
	m_states[PlayerStateKind::kEquipKnife]				= std::make_shared<player_state::EquipKnife>			(m_player, *this, animator);
	m_states[PlayerStateKind::kReload]					= std::make_shared<player_state::Reload>				(m_player, *this, animator);
	m_states[PlayerStateKind::kShot]					= std::make_shared<player_state::Shot>					(m_player, *this, animator);
	m_states[PlayerStateKind::kShotRocketLauncher]		= std::make_shared<player_state::ShotRocketLauncher>	(m_player, *this, animator);
	m_states[PlayerStateKind::kFirstSideSlashKnife]		= std::make_shared<player_state::FirstSideSlashKnife>	(m_player, *this, animator);
	m_states[PlayerStateKind::kSecondSideSlashKnife]	= std::make_shared<player_state::SecondSideSlashKnife>	(m_player, *this, animator);
	m_states[PlayerStateKind::kSpinningSlashKnife]		= std::make_shared<player_state::SpinningSlashKnife>	(m_player, *this, animator);
	m_states[PlayerStateKind::kFrontKick]				= std::make_shared<player_state::FrontKick>				(m_player, *this, animator);
	m_states[PlayerStateKind::kRoundhouseKick]			= std::make_shared<player_state::RoundhouseKick>		(m_player, *this, animator);
	m_states[PlayerStateKind::kEscape]					= std::make_shared<player_state::Escape>				(m_player, *this, animator);
	m_states[PlayerStateKind::kGrabbed]					= std::make_shared<player_state::Grabbed>				(m_player, *this, animator);
	m_states[PlayerStateKind::kStealthKill]				= std::make_shared<player_state::StealthKill>			(m_player, *this, animator);
	m_states[PlayerStateKind::kVictoryPose]				= std::make_shared<player_state::VictoryPose>			(m_player, *this, animator);

	m_current_state = m_states.at(PlayerStateKind::kIdle);
	m_current_state->Enter();
}

player_state::State::~State()
{

}

void player_state::State::Update()
{
	// 次のステートがNoneでない場合はステートを変更
	const auto next_state_kind = m_current_state->GetNextStateKind();
	if (next_state_kind != PlayerStateKind::kNone)
	{
		m_prev_state_kind = m_current_state->GetStateKind();
		m_current_state->Exit();
		m_current_state = m_states.at(next_state_kind);
		m_current_state->Enter();
	}

	m_current_state->Update();
}

void player_state::State::LateUpdate()
{
	m_current_state->LateUpdate();
}

const PlayerStateKind player_state::State::GetPrevStateKind() const
{
	return m_prev_state_kind;
}

const PlayerStateKind player_state::State::GetCurrentStateKind() const
{
	return m_current_state->GetStateKind();
}


#pragma region Try判定
const bool player_state::State::TryDead()
{
	return !m_player.IsAlive();
}

const bool player_state::State::TryGrabbed()
{
	return m_player.IsGrabbed();
}

const bool player_state::State::TryFrontKick()
{
	if (!m_player.CanControl()) { return false; }
	if (!m_player.GetMeleeTarget()) { return false; }
	if (!CommandHandler::GetInstance()->IsExecute(CommandKind::kMelee, TimeKind::kCurrent)) { return false; }

	// ターゲットをメレー対象となるインターフェイスに変更
	const auto melee_target = m_player.GetMeleeTarget();
	const auto target_obj	= std::dynamic_pointer_cast<const ObjBase>(m_player.GetMeleeTarget());

	// 怯み中(しゃがみ)でない場合は遷移を許可しない
	if (!melee_target->IsCrouchStun()) { return false; }

	// プレイヤーがターゲットの前方にいない場合は遷移を許可しない
	const auto target_forward = target_obj->GetTransform()->GetForward(CoordinateKind::kWorld);
	auto dir = m_player.GetTransform()->GetPos(CoordinateKind::kWorld) - target_obj->GetTransform()->GetPos(CoordinateKind::kWorld);
	dir.y = 0.0f;
	dir = v3d::GetNormalizedV(dir);
	if (math::GetAngleBetweenTwoVectors(target_forward, dir) > 45.0f * math::kDegToRad) { return false; }

	// プレイヤーの入力に即座に対応できるようにmove_dirを基準になす角を取得
	const auto player_move_dir	= m_player.GetMoveDir(TimeKind::kCurrent);
	const auto player_dir		= player_move_dir != v3d::GetZeroV() ? player_move_dir : m_player.GetLookDir(TimeKind::kCurrent);
	const auto forward_angle	= math::GetAngleBetweenTwoVectors(target_forward, player_dir);

	// 正面から蹴った場合にステートを遷移
	return forward_angle >= 135.0f * math::kDegToRad;
}

const bool player_state::State::TryRoundhouseKick()
{
	if (!m_player.CanControl()) { return false; }
	if (!m_player.GetMeleeTarget()) { return false; }
	if (!CommandHandler::GetInstance()->IsExecute(CommandKind::kMelee, TimeKind::kCurrent)) { return false; }

	// ターゲットをメレー対象となるインターフェイスに変更
	const auto melee_target = m_player.GetMeleeTarget();
	const auto target_obj = std::dynamic_pointer_cast<const ObjBase>(m_player.GetMeleeTarget());

	return melee_target->IsStandStun() || melee_target->IsCrouchStun();
}

const bool player_state::State::TryStealthKill()
{
	if (!m_player.CanControl()) { return false; }
	if (!m_player.GetStealthKillTarget()) { return false; }
	if (!CommandHandler::GetInstance()->IsExecute(CommandKind::kAttack, TimeKind::kCurrent)) { return false; }

	return true;
}

const bool player_state::State::TryEquipKnifeShortcut()
{
	const auto weapon = m_player.GetCurrentHeldWeapon();

	if (weapon)
	{
		return weapon->GetWeaponKind() == WeaponKind::kKnife;
	}
	return false;
}

const bool player_state::State::TryAimKnife()
{
	if (!m_player.CanControl()) { return false; }
	if (!m_player.IsAlive()) { return false; }

	const auto command = CommandHandler::GetInstance();
	return command->IsExecute(CommandKind::kAimKnife, TimeKind::kCurrent) && m_player.GetCurrentEquipWeapon(WeaponSlotKind::kSub);
}

const bool player_state::State::TryFirstSideSlashKnife()
{
	if (!m_player.CanControl()) { return false; }
	if (!m_player.IsAlive()) { return false; }

	const auto command = CommandHandler::GetInstance();
	return command->IsExecute(CommandKind::kAttack, TimeKind::kCurrent);
}

const bool player_state::State::TrySpinningSlash()
{
	if (!m_player.CanControl()) { return false; }

	return  m_current_state->IsRun() && CommandHandler::GetInstance()->IsExecute(CommandKind::kAttack, TimeKind::kCurrent);
}

const bool player_state::State::TryEquipGun()
{
	if (!m_player.CanControl()) { return false; }

	return m_player.GetCurrentEquipWeaponKind(WeaponSlotKind::kMain) == WeaponKind::kGun
		&& CommandHandler::GetInstance()->IsExecute(CommandKind::kAimGun, TimeKind::kCurrent);
}

const bool player_state::State::TryEquipGunShortcut()
{
	const auto weapon = m_player.GetCurrentHeldWeapon();

	if (weapon)
	{
		return weapon->GetWeaponKind() == WeaponKind::kGun;
	}

	return false;
}

const bool player_state::State::TryPullTrigger()
{
	if (!m_player.CanControl()) { return false; }

	const auto gun = std::dynamic_pointer_cast<GunBase>(m_player.GetCurrentHeldWeapon());

	if (gun)
	{
		if (CommandHandler::GetInstance()->IsExecute(CommandKind::kPullTrigger, TimeKind::kCurrent))
		{
			gun->PullTrigger();
			return true;
		}
		else
		{
			gun->ReleaseTrigger();
		}
	}

	return false;
}

const bool player_state::State::TryReload()
{
	if (!m_player.CanControl()) { return false; }
	if (!m_player.IsAlive()) { return false; }

	const auto gun = std::dynamic_pointer_cast<GunBase>(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));
	const auto ammo_holder = m_player.GetAmmoHolder();

	if (gun)
	{
		switch (gun->GetGunKind())
		{
		case GunKind::kAssaultRifle:
			if (ammo_holder->GetCurrentAmmoNum(typeid(AssaultRifleAmmoBox)) <= 0) { return false; }
			break;

		case GunKind::kRocketLauncher:
			if (ammo_holder->GetCurrentAmmoNum(typeid(RocketBombBox)) <= 0) { return false; }
			break;

		default:
			break;
		}

		return gun->CanReload() && CommandHandler::GetInstance()->IsExecute(CommandKind::kReload, TimeKind::kCurrent);
	}

	return false;
}

const bool player_state::State::TryPullTriggerReload()
{
	if (!m_player.CanControl()) { return false; }

	const auto gun = std::dynamic_pointer_cast<GunBase>(m_player.GetCurrentHeldWeapon());
	if (!gun) { return false; }
	if (gun->GetCurrentRemainingBulletNum() > 0) { return false; }

	const auto command = CommandHandler::GetInstance();
	const auto ammo_holder = m_player.GetAmmoHolder();
	if (gun)
	{
		switch (gun->GetGunKind())
		{
		case GunKind::kAssaultRifle:
			if (ammo_holder->GetCurrentAmmoNum(typeid(AssaultRifleAmmoBox)) <= 0) { return false; }
			break;

		case GunKind::kRocketLauncher:
			if (ammo_holder->GetCurrentAmmoNum(typeid(RocketBombBox)) <= 0) { return false; }
			break;

		default:
			break;
		}

		return !command->IsExecute(CommandKind::kPullTrigger, TimeKind::kPrev) && command->IsExecute(CommandKind::kPullTrigger, TimeKind::kCurrent);
	}

	return false;
}
#pragma endregion