#include "roundhouse_kick.hpp"

player_state::RoundhouseKick::RoundhouseKick() :
	ActionStateBase	(static_cast<int>(player_state::ActionStateKind::kRoundhouseKick)),
	m_is_all_stop	(true)
{

}

player_state::RoundhouseKick::~RoundhouseKick()
{

}

void player_state::RoundhouseKick::Update(std::shared_ptr<Player> obj)
{

}

void player_state::RoundhouseKick::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::RoundhouseKick::Enter(std::shared_ptr<Player> obj)
{

}

void player_state::RoundhouseKick::Exit(std::shared_ptr<Player> obj)
{

}

std::shared_ptr<IState<Player>> player_state::RoundhouseKick::ChangeState(std::shared_ptr<Player> obj)
{
	const auto state_controller = obj->GetStateController();

	// NULL
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<ActionNull, Player>();
	}

	return nullptr;
}
