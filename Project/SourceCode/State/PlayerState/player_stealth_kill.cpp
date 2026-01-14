#include "../../Object/player.hpp"
#include "../../Kind/player_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Event/event_system.hpp"
#include "../../Kind/player_state_kind.hpp"
#include "../../VirtualCamera/stealth_kill_virtual_camera_controller.hpp"
#include "player_state.hpp"
#include "player_stealth_kill.hpp"

player_state::StealthKill::StealthKill(Player& player, State& state, const std::shared_ptr<Animator>& animator) :
    PlayerStateBase                 (player, state, animator, PlayerStateKind::kStealthKill),
    m_is_stab                       (false),
    m_is_draw                       (false),
    m_stealth_kill_camera_controller(nullptr)
{
}

player_state::StealthKill::~StealthKill()
{
}

void player_state::StealthKill::Update()
{
    m_animator->AttachResultAnim(static_cast<int>(PlayerAnimKind::kStealthKill));

    m_player.UpdateStealthKill();

    const auto play_rate = m_player.GetAnimator()->GetPlayRate(Animator::BodyKind::kUpperBody);
    const auto event_system = EventSystem::GetInstance();

    if (play_rate > 0.5f && !m_is_stab)
    {
        event_system->Publish(StabKnifeEvent(m_player.GetCurrentHeldWeapon()->GetTransform()->GetPos(CoordinateKind::kWorld), TimeScaleLayerKind::kPlayer));
        m_is_stab = true;
    }

    if (play_rate > 0.7f && !m_is_draw)
    {
        event_system->Publish(DrawKnifeEvent(m_player.GetCurrentHeldWeapon()->GetTransform()->GetPos(CoordinateKind::kWorld), TimeScaleLayerKind::kPlayer));
        m_is_draw = true;
    }
}

void player_state::StealthKill::LateUpdate()
{
    m_player.OnFootIK();
}

void player_state::StealthKill::Enter()
{
    m_is_stab = false;
    m_is_draw = false;

    const auto cinemachine_brain = CinemachineBrain::GetInstance();
    m_stealth_kill_camera_controller = std::make_shared<StealthKillVirtualCameraController>();
    cinemachine_brain->AddVirtualCameraController(m_stealth_kill_camera_controller);

    m_player.SetupStealthKill();
}

void player_state::StealthKill::Exit()
{
    const auto cinemachine_brain = CinemachineBrain::GetInstance();
    cinemachine_brain->RemoveVirtualCameraController(m_stealth_kill_camera_controller);
    m_stealth_kill_camera_controller = nullptr;

    m_player.ReleaseWeapon();
    m_player.AttachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

const PlayerStateKind player_state::StealthKill::GetNextStateKind()
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
