#include "run.hpp"

player_state::Run::Run() :
	ActionStateBase(static_cast<int>(player_state::ActionStateKind::kRun))
{

}

player_state::Run::~Run()
{

}

void player_state::Run::Update(Player* obj)
{

}

void player_state::Run::Enter(const Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::Run::ChangeState(const Player* obj)
{
	const auto state_controller = obj->GetStateController();

	if (state_controller->TryCrouch())
	{
		return state_controller->GetState<Crouch, Player>();
	}

	return nullptr;
}
