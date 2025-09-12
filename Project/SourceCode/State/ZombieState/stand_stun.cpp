#include "stand_stun.hpp"

zombie_state::StandStun::StandStun() :
	ActionStateBase(static_cast<int>(zombie_state::ActionStateKind::kStandStun))
{

}

zombie_state::StandStun::~StandStun()
{

}

void zombie_state::StandStun::Update(std::shared_ptr<Zombie> obj)
{
	obj->CalcAttackIntervalTime();
}

void zombie_state::StandStun::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::StandStun::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::StandStun::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::StandStun::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
