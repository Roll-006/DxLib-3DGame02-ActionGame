#include "victory_pose.hpp"

player_state::VictoryPose::VictoryPose() :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kVictoryPose)),
	m_is_stop_all_state	(true)
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

}

void player_state::VictoryPose::Exit(std::shared_ptr<Player>& obj)
{

}

std::shared_ptr<IState<Player>> player_state::VictoryPose::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	return nullptr;
}
