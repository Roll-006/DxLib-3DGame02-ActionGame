#include "../../Object/player.hpp"
#include "../../Kind/player_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Kind/player_state_kind.hpp"
#include "../../Kind/player_anim_kind.hpp"
#include "player_state.hpp"
#include "player_roundhouse_kick.hpp"

player_state::RoundhouseKick::RoundhouseKick(Player& player, State& state, const std::shared_ptr<Animator>& animator) :
    PlayerStateBase(player, state, animator, PlayerStateKind::kRoundhouseKick),
    m_has_trigger_created(false),
    m_has_trigger_deleted(false)
{
}

player_state::RoundhouseKick::~RoundhouseKick()
{
}

void player_state::RoundhouseKick::Update()
{
    m_animator->AttachResultAnim(static_cast<int>(PlayerAnimKind::kRoundhouseKick));

    m_player.UpdateMelee();

    const auto anim_kind = static_cast<PlayerAnimKind>(m_animator->GetAnimKind(Animator::BodyKind::kUpperBody, TimeKind::kCurrent));
    const auto play_rate = m_animator->GetPlayRate(Animator::BodyKind::kUpperBody);

    // 攻撃判定用トリガーを追加
    if (!m_has_trigger_created && play_rate > 0.35f && anim_kind == PlayerAnimKind::kRoundhouseKick)
    {
        m_player.AddCollider(std::make_shared<Collider>(ColliderKind::kAttackTrigger, std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), kAttackTriggerRadius), &m_player));
        m_has_trigger_created = true;
    }

    // 攻撃判定用トリガーを削除
    if (!m_has_trigger_deleted && play_rate > 0.8f && anim_kind == PlayerAnimKind::kRoundhouseKick)
    {
        m_player.RemoveCollider(ColliderKind::kAttackTrigger);
        m_has_trigger_deleted = true;
    }

    // 座標計算
    if (m_has_trigger_created && !m_has_trigger_deleted)
    {
        m_player.GetModeler()->ApplyMatrix();

        const auto model_handle     = m_player.GetModeler()->GetModelHandle();
        const auto right_leg_m      = MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, FramePath.RIGHT_LEG));
        const auto right_foot_m     = MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, FramePath.RIGHT_FOOT));
        const auto right_leg_pos    = matrix::GetPos(right_leg_m);
        const auto right_foot_pos   = matrix::GetPos(right_foot_m);

        const auto capsule = std::static_pointer_cast<Capsule>(m_player.GetCollider(ColliderKind::kAttackTrigger)->GetShape());
        capsule->SetSegmentBeginPos(right_leg_pos, true);
        capsule->SetSegmentEndPos(right_foot_pos, true);
    }
}

void player_state::RoundhouseKick::LateUpdate()
{
}

void player_state::RoundhouseKick::Enter()
{
    m_has_trigger_created = false;
    m_has_trigger_deleted = false;

    m_player.SetupVersatilityMelee();
}

void player_state::RoundhouseKick::Exit()
{
}

const PlayerStateKind player_state::RoundhouseKick::GetNextStateKind()
{
    if (m_player.GetDeltaTime() <= 0.0f)
    {
        return PlayerStateKind::kNone;
    }
    // IDLE
    else if (m_player.GetAnimator()->IsPlayEnd(Animator::BodyKind::kUpperBody))
    {
        return PlayerStateKind::kIdle;
    }

    return PlayerStateKind::kNone;
}
