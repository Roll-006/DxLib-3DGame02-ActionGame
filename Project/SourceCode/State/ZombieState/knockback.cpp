#include "knockback.hpp"

zombie_state::Knockback::Knockback() :
	ActionStateBase	(static_cast<int>(zombie_state::ActionStateKind::kKnockback)),
	m_is_all_stop	(false)
{

}

zombie_state::Knockback::~Knockback()
{

}

void zombie_state::Knockback::Update(std::shared_ptr<Zombie> obj)
{
	obj->CalcAttackIntervalTime();
}

void zombie_state::Knockback::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Knockback::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Knockback::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Knockback::ChangeState(std::shared_ptr<Zombie> obj)
{
	const auto state_controller = obj->GetStateController();

	// 死亡
	if (state_controller->TryDead(obj))
	{
		return state_controller->GetState<Dead, Zombie>();
	}
	// 左足ダウン
	if (state_controller->TryLeftCrouchStun(obj))
	{
		return state_controller->GetState<CrouchLeftStun, Zombie>();
	}
	// 右足ダウン
	if (state_controller->TryRightCrouchStun(obj))
	{
		return state_controller->GetState<CrouchRightStun, Zombie>();
	}
	// 立ちダウン
	if (state_controller->TryStandStun(obj))
	{
		return state_controller->GetState<StandStun, Zombie>();
	}

	return nullptr;
}
