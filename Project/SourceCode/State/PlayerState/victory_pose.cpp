#include "victory_pose.hpp"

player_state::VictoryPose::VictoryPose() :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kVictoryPose)),
	m_is_stop_all_state	(true),
	m_camera_controller	(nullptr)
{

}

player_state::VictoryPose::~VictoryPose()
{

}

void player_state::VictoryPose::Update(std::shared_ptr<Player>& obj)
{
	
}

void player_state::VictoryPose::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::VictoryPose::Enter(std::shared_ptr<Player>& obj)
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	m_camera_controller = std::make_shared<GameClearVirtualCamerasController>(obj->GetModeler()->GetModelHandle(), obj->GetTransform());
	cinemachine_brain->AddVirtualCameraController(m_camera_controller);

	const VictoryPoseEvent event{ obj->GetTransform(), obj->GetModeler() };
	EventSystem::GetInstance()->Publish(event);
}

void player_state::VictoryPose::Exit(std::shared_ptr<Player>& obj)
{
	// ‰‰o—pƒJƒƒ‰‚ðíœ
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCameraController(m_camera_controller);
	m_camera_controller = nullptr;
}

std::shared_ptr<IState<Player>> player_state::VictoryPose::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	return nullptr;
}
