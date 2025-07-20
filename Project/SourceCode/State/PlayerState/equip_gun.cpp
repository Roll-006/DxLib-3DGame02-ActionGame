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

	// 入力されてから一定時間経過でエイミング状態へ移行
	if (CommandHandler::GetInstance()->IsExecutingCommand(CommandKind::kAimGun))
	{
		m_possible_aim_timer += FPS::GetDeltaTime();
		if (m_possible_aim_timer >= kPossibleAimTime)
		{
			return state_controller->GetState<AimGun, Player>();
		}
	}
	else
	{
		m_possible_aim_timer = 0.0f;
	}

	return nullptr;
}
