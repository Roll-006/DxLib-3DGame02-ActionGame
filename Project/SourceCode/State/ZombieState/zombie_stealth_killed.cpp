#include "../../Object/zombie.hpp"
#include "../../Kind/zombie_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Kind/zombie_state_kind.hpp"
#include "../../Event/event_system.hpp"
#include "zombie_state.hpp"
#include "zombie_stealth_killed.hpp"

zombie_state::StealthKilled::StealthKilled(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator) :
	ZombieStateBase(zombie, state, animator, ZombieStateKind::kStealthKilled)
{

}

zombie_state::StealthKilled::~StealthKilled()
{

}

void zombie_state::StealthKilled::Update()
{
	m_zombie.CalcMoveSpeedStop();
	m_zombie.DisallowStealthKill();

	m_zombie.UpdateLocomotion();

	m_animator->AttachResultAnim(static_cast<int>(ZombieAnimKind::kStealthKilled));
}

void zombie_state::StealthKilled::LateUpdate()
{

}

void zombie_state::StealthKilled::Enter()
{
	// ステルスキルされたことを演出カメラに通知
	const OnStealthKillEvent event{ m_zombie.GetEnemyID(), m_zombie.GetModeler() };
	EventSystem::GetInstance()->Publish(event);
}

void zombie_state::StealthKilled::Exit()
{
	m_zombie.ExitStealthKilled();
}

const ZombieStateKind zombie_state::StealthKilled::GetNextStateKind()
{
	if (m_zombie.GetDeltaTime() <= 0.0f)
	{
		return ZombieStateKind::kNone;
	}

	// 上半身アニメーション終了待ち
	if (m_animator->IsPlayEnd(Animator::BodyKind::kUpperBody))
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