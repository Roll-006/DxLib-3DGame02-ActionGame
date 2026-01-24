#include "../../Object/zombie.hpp"
#include "../../Kind/zombie_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Kind/zombie_state_kind.hpp"
#include "zombie_state.hpp"
#include "zombie_backward_knockback.hpp"

zombie_state::BackwardKnockback::BackwardKnockback(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator) :
	ZombieStateBase(zombie, state, animator, ZombieStateKind::kBackwardKnockback)
{

}

zombie_state::BackwardKnockback::~BackwardKnockback()
{

}

void zombie_state::BackwardKnockback::Update()
{
	m_zombie.CalcMoveSpeedStop();

	m_zombie.ActivateInvincibleForcibly();
	m_zombie.DisallowDecreaseKnockBackGauge();
	m_zombie.DisallowStealthKill();
	m_zombie.DisallowDisperseEnemy();

	m_zombie.UpdateLocomotion();

	m_animator->AttachResultAnim(static_cast<int>(ZombieAnimKind::kBackwardKnockback));
}

void zombie_state::BackwardKnockback::LateUpdate()
{

}

void zombie_state::BackwardKnockback::Enter()
{
	const auto knock_back_gauge = m_zombie.GetKnockBackGauge();
	if (!knock_back_gauge->IsAlive())
	{
		knock_back_gauge->IncreaseCurrentMax();
	}
}

void zombie_state::BackwardKnockback::Exit()
{

}

const ZombieStateKind zombie_state::BackwardKnockback::GetNextStateKind()
{
	if (m_zombie.GetDeltaTime() <= 0.0f)
	{
		return ZombieStateKind::kNone;
	}
	// ノックバック終了
	else if (m_zombie.GetKnockBackSpeed() < math::kEpsilonLow)
	{
		// 死亡
		if (m_state.TryDead())
		{
			return ZombieStateKind::kDead;
		}


		// NULL
		return ZombieStateKind::kIdle;
	}

	return ZombieStateKind::kNone;
}
