#include "../../Object/zombie.hpp"
#include "../../Kind/zombie_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Kind/zombie_state_kind.hpp"
#include "zombie_state.hpp"
#include "zombie_search.hpp"

zombie_state::Search::Search(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator) :
	ZombieStateBase(zombie, state, animator, ZombieStateKind::kSearch)
{

}

zombie_state::Search::~Search()
{

}

void zombie_state::Search::Update()
{

}

void zombie_state::Search::LateUpdate()
{

}

void zombie_state::Search::Enter()
{

}

void zombie_state::Search::Exit()
{

}

const ZombieStateKind zombie_state::Search::GetNextStateKind()
{
	//if (m_zombie.GetDeltaTime() <= 0.0f)
	//{
	//	return ZombieStateKind::kNone;
	//}
	//// 強制待機
	//else if (m_state.TryWaitForcibly())
	//{
	//	return ZombieStateKind::kIdle;
	//}

	return ZombieStateKind::kNone;
}
