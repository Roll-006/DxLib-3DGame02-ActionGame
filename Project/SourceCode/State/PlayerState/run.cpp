#include "run.hpp"

player_state::Run::Run()
{

}

player_state::Run::~Run()
{

}

void player_state::Run::Update(const Player* obj)
{

}

void player_state::Run::Enter(const Player* obj)
{

}

IState<Player>* player_state::Run::ChangeState(const Player* obj)
{
	auto test = Run();
	return &test;
}
