#include "../../Object/player.hpp"
#include "../../Kind/player_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Event/event_system.hpp"
#include "../../Kind/player_state_kind.hpp"
#include "player_state.hpp"
#include "player_victory_pose.hpp"

player_state::VictoryPose::VictoryPose(Player& player, State& state, const std::shared_ptr<Animator>& animator) :
    PlayerStateBase(player, state, animator, PlayerStateKind::kVictoryPose),
    m_camera_controller(nullptr)
{
}

player_state::VictoryPose::~VictoryPose()
{
}

void player_state::VictoryPose::Update()
{
    const auto current_lower_anim_kind = m_animator->GetAnimKind(Animator::BodyKind::kLowerBody, TimeKind::kCurrent);
    if (current_lower_anim_kind == static_cast<int>(PlayerAnimKind::kStandToCrouch) || current_lower_anim_kind == static_cast<int>(PlayerAnimKind::kTalkingCrouch))
    {
        if (m_animator->IsPlayEnd(Animator::BodyKind::kLowerBody))
        {
            m_animator->AttachAnim(static_cast<int>(PlayerAnimKind::kTalkingCrouch),  Animator::BodyKind::kLowerBody);
            m_animator->AttachAnim(static_cast<int>(PlayerAnimKind::kTalkingOnPhone), Animator::BodyKind::kUpperBody);
        }
    }
    else
    {
        m_animator->AttachResultAnim(static_cast<int>(PlayerAnimKind::kStandToCrouch));
    }
}

void player_state::VictoryPose::LateUpdate()
{
}

void player_state::VictoryPose::Enter()
{
    const auto cinemachine_brain = CinemachineBrain::GetInstance();
    m_camera_controller = std::make_shared<GameClearVirtualCamerasController>(m_player.GetModeler()->GetModelHandle(), m_player.GetTransform());
    cinemachine_brain->AddVirtualCameraController(m_camera_controller);

    const VictoryPoseEvent event{ m_player.GetTransform(), m_player.GetModeler() };
    EventSystem::GetInstance()->Publish(event);
}

void player_state::VictoryPose::Exit()
{
    const auto cinemachine_brain = CinemachineBrain::GetInstance();
    cinemachine_brain->RemoveVirtualCameraController(m_camera_controller);
    m_camera_controller = nullptr;
}

const PlayerStateKind player_state::VictoryPose::GetNextStateKind()
{
    return PlayerStateKind::kNone;
}
