#include "victory_pose.hpp"

player_state::VictoryPose::VictoryPose(Player& player) :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kVictoryPose)),
	m_is_stop_all_state	(true),
	m_camera_controller	(nullptr),
	m_player			(player)
{

}

player_state::VictoryPose::~VictoryPose()
{

}

void player_state::VictoryPose::Update()
{
	
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
	// 演出用カメラを削除
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCameraController(m_camera_controller);
	m_camera_controller = nullptr;
}

int player_state::VictoryPose::GetNextStateKind()
{
	if (m_player.GetDeltaTime() <= 0.0f) { return static_cast<int>(player_state::ActionStateKind::kNone); }

	return static_cast<int>(player_state::ActionStateKind::kNone);
}
