#include "equip_gun.hpp"

player_state::EquipGun::EquipGun() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kEquipGun)),
	m_possible_aim_timer	(0.0f)
{

}

player_state::EquipGun::~EquipGun()
{

}

void player_state::EquipGun::Update(Player* obj)
{
	if (CommandHandler::GetInstance()->IsExecuting(CommandKind::kAimGun))
	{
		m_possible_aim_timer += FPS::GetDeltaTime();
	}
	else
	{
		m_possible_aim_timer = 0.0f;
	}
}

void player_state::EquipGun::LateUpdate(Player* obj)
{

}

void player_state::EquipGun::Enter(Player* obj)
{
	m_possible_aim_timer = 0.0f;
}

void player_state::EquipGun::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::EquipGun::ChangeState(const Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	// 銃エイミング状態
	if (CommandHandler::GetInstance()->IsExecuting(CommandKind::kAimGun))
	{	
		if (m_possible_aim_timer >= kPossibleAimTime)
		{
			return state_controller->GetState<AimGun, Player>();
		}
	}
	// ナイフエイミング状態
	if (command->IsExecuting(CommandKind::kAimKnife))
	{
		return state_controller->GetState<AimKnife, Player>();
	}
	// 切り裂く(第一段階)
	if (command->IsExecuting(CommandKind::kAttack))
	{
		return state_controller->GetState<FirstSideSlashKnife, Player>();
	}

	return nullptr;
}
