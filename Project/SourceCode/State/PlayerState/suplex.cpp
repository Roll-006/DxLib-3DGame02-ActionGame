#include "suplex.hpp"

player_state::Suplex::Suplex() :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kSuplex)),
	m_is_stop_all_state	(false)
{

}

player_state::Suplex::~Suplex()
{

}

void player_state::Suplex::Update(std::shared_ptr<Player>& obj)
{
	obj->UpdateMelee();
}

void player_state::Suplex::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::Suplex::Enter(std::shared_ptr<Player>& obj)
{

}

void player_state::Suplex::Exit(std::shared_ptr<Player>& obj)
{

}

std::shared_ptr<IState<Player>> player_state::Suplex::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	return nullptr;
}
