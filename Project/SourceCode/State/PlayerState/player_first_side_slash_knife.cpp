#include "../../Object/player.hpp"
#include "../../Kind/player_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Event/event_system.hpp"
#include "../../Kind/player_state_kind.hpp"
#include "../../Kind/player_anim_kind.hpp"
#include "../../Command/command_handler.hpp"
#include "player_state.hpp"
#include "player_first_side_slash_knife.hpp"

player_state::FirstSideSlashKnife::FirstSideSlashKnife(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator) :
	PlayerStateBase(player, state, animator, PlayerStateKind::kFirstSideSlashKnife),
	m_combo_timer(0.0f),
	m_has_trigger_created(false),
	m_has_trigger_deleted(false)
{
}

player_state::FirstSideSlashKnife::~FirstSideSlashKnife()
{
}

void player_state::FirstSideSlashKnife::Update()
{
	const auto command = CommandHandler::GetInstance();
	command->InitCurrentTriggerInputCount(CommandKind::kCrouch);
	command->InitCurrentTriggerInputCount(CommandKind::kRun);

	m_animator->AttachResultAnim(static_cast<int>(PlayerAnimKind::kFirstSideSlashKnife));

	const auto time_manager = GameTimeManager::GetInstance();
	m_combo_timer += time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);

	const auto anim_kind = static_cast<PlayerAnimKind>(m_animator->GetAnimKind(Animator::BodyKind::kUpperBody, TimeKind::kCurrent));
	const auto play_rate = m_animator->GetPlayRate(Animator::BodyKind::kUpperBody);

	m_player.AllowCalcLookDir();
	m_player.GetCurrentHeldWeapon()->Update();
	m_player.SideSlashKnifeOffsetMove();

	// 攻撃判定生成
	if (!m_has_trigger_created && play_rate > 0.3f && anim_kind == PlayerAnimKind::kFirstSideSlashKnife)
	{
		const auto knife = std::dynamic_pointer_cast<KnifeBase>(m_player.GetCurrentHeldWeapon());
		if (knife)
		{
			knife->AddAttackTrigger();
			m_has_trigger_created = true;

			EventSystem::GetInstance()->Publish(
				FirstSlashEvent(knife->GetTransform()->GetPos(CoordinateKind::kWorld), TimeScaleLayerKind::kPlayer));
		}
	}

	// 攻撃判定削除
	if (!m_has_trigger_deleted && play_rate > 0.8f && anim_kind == PlayerAnimKind::kFirstSideSlashKnife)
	{
		const auto knife = std::dynamic_pointer_cast<KnifeBase>(m_player.GetCurrentHeldWeapon());
		if (knife)
		{
			knife->RemoveAttackTrigger();
			m_has_trigger_deleted = true;
		}
	}
}

void player_state::FirstSideSlashKnife::LateUpdate()
{
}

void player_state::FirstSideSlashKnife::Enter()
{
	m_combo_timer = 0.0f;
	m_has_trigger_created = false;
	m_has_trigger_deleted = false;

	m_player.DetachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kSub));
	m_player.HoldWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

void player_state::FirstSideSlashKnife::Exit()
{
	m_player.ReleaseWeapon();
	m_player.AttachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

const PlayerStateKind player_state::FirstSideSlashKnife::GetNextStateKind()
{
	if (m_player.GetDeltaTime() <= 0.0f)
	{
		return PlayerStateKind::kNone;
	}
	// 切り裂く(第二段階)
	else if (m_player.CanControl() && m_combo_timer > kComboValidTime && CommandHandler::GetInstance()->IsExecute(CommandKind::kAttack, TimeKind::kCurrent))
	{
		return PlayerStateKind::kSecondSideSlashKnife;
	}
	// ナイフ装備状態
	else if (m_animator->IsPlayEnd(Animator::BodyKind::kUpperBody))
	{
		return PlayerStateKind::kEquipKnife;
	}

	return PlayerStateKind::kNone;
}
