#include <memory>
#include "../Base/character_base.hpp"
#include "zombie_ai.hpp"

ZombieAI::ZombieAI()
{

}

ZombieAI::~ZombieAI()
{

}

void ZombieAI::AttachTarget(const std::shared_ptr<CharacterBase>& target)
{
	m_target = target;
}

void ZombieAI::DetachTarget()
{
	m_target = nullptr;
}
