#include "knife_equipped.hpp"

player_state::KnifeEquipped::KnifeEquipped()
{

}

player_state::KnifeEquipped::~KnifeEquipped()
{

}

void player_state::KnifeEquipped::Update(const Player* obj)
{

}

void player_state::KnifeEquipped::Enter(const Player* obj)
{

}

IState<Player>* player_state::KnifeEquipped::ChangeState(const Player* obj)
{
	auto test = KnifeEquipped();
	return &test;
}
