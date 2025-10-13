#include "player_dead.hpp"

player_state::Dead::Dead() :
	ActionStateBase				(static_cast<int>(player_state::ActionStateKind::kDead)),
	m_is_stop_all_state			(false),
	m_dead_cameras_controller	(nullptr)
{

}

player_state::Dead::~Dead()
{

}

void player_state::Dead::Update(std::shared_ptr<Player>& obj)
{

}

void player_state::Dead::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::Dead::Enter(std::shared_ptr<Player>& obj)
{
	const auto cinemachine_brain	= CinemachineBrain::GetInstance();
	m_dead_cameras_controller		= std::make_shared<DeadVirtualCamerasController>();
	cinemachine_brain->AddVirtualCameraController(m_dead_cameras_controller);
}

void player_state::Dead::Exit(std::shared_ptr<Player>& obj)
{
	// ‰‰o—pƒJƒƒ‰‚ðíœ
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCameraController(m_dead_cameras_controller);
	m_dead_cameras_controller = nullptr;
}

std::shared_ptr<IState<Player>> player_state::Dead::ChangeState(std::shared_ptr<Player>& obj)
{
	// Ž€–S‚©‚ç‚Ì‘JˆÚ‚Í‚È‚¢
	return nullptr;
}
