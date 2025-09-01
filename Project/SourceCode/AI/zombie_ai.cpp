#include "zombie_ai.hpp"

ZombieAI::ZombieAI(const std::shared_ptr<Player> player) : 
	m_state	(std::make_shared<ZombieStateController>()),
	m_player(player)
{

}

ZombieAI::~ZombieAI()
{

}

void ZombieAI::Update(const std::shared_ptr<Zombie> enemy)
{
	m_state->Update(enemy);
}

void ZombieAI::LateUpdate(const std::shared_ptr<Zombie> enemy)
{
	m_state->LateUpdate(enemy);
}
