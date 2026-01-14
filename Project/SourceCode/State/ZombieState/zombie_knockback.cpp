#include "../../Object/zombie.hpp"
#include "../../Kind/zombie_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Kind/zombie_state_kind.hpp"
#include "zombie_state.hpp"
#include "zombie_knockback.hpp"

zombie_state::Knockback::Knockback(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator) :
	ZombieStateBase(zombie, state, animator, ZombieStateKind::kKnockback)
{

}

zombie_state::Knockback::~Knockback()
{

}

void zombie_state::Knockback::Update()
{
	if (m_zombie.CanAction())
	{
		m_zombie.CalcAttackIntervalTime();
	}

	m_zombie.CalcMoveSpeedStop();
	m_zombie.DisallowStealthKill();
	m_zombie.DisallowDecreaseKnockBackGauge();

	m_zombie.UpdateLocomotion();

	m_animator->AttachResultAnim(static_cast<int>(ZombieAnimKind::kFlyingKnockbackDown));
}

void zombie_state::Knockback::LateUpdate()
{
	m_zombie.DisallowProjectPos();
}

void zombie_state::Knockback::Enter()
{

}

void zombie_state::Knockback::Exit()
{

}

const ZombieStateKind zombie_state::Knockback::GetNextStateKind()
{
	if (m_zombie.GetDeltaTime() <= 0.0f)
	{
		return ZombieStateKind::kNone;
	}

	if (m_zombie.GetKnockBackSpeed() < math::kEpsilonLow)
	{
		// 死亡
		if (m_state.TryDead())
		{
			return ZombieStateKind::kDead;
		}

		// 立ち上がり
		return ZombieStateKind::kStandUp;
	}

	return ZombieStateKind::kNone;
}
