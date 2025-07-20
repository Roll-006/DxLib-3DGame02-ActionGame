#include "aim_gun.hpp"

player_state::AimGun::AimGun() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kAimGun))
{
	m_stop_states.emplace_back();
}

player_state::AimGun::~AimGun()
{

}

void player_state::AimGun::Update(Player* obj)
{
	obj->SetLookDirCorrectValueForAim();
}

void player_state::AimGun::LateUpdate(Player* obj)
{

}

void player_state::AimGun::Enter(Player* obj)
{

}

void player_state::AimGun::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::AimGun::ChangeState(const Player* obj)
{
	const auto state_controller = obj->GetStateController();

	if (!CommandHandler::GetInstance()->IsExecutingCommand(CommandKind::kAimGun))
	{
		return state_controller->GetState<EquipGun, Player>();
	}

	return nullptr;
}
