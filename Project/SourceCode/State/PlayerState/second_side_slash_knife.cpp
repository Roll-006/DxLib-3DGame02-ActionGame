#include "second_side_slash_knife.hpp"

player_state::SecondSideSlashKnife::SecondSideSlashKnife() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kSecondSideSlashKnife))
{

}

player_state::SecondSideSlashKnife::~SecondSideSlashKnife()
{

}

void player_state::SecondSideSlashKnife::Update(Player* obj)
{

}

void player_state::SecondSideSlashKnife::LateUpdate(Player* obj)
{

}

void player_state::SecondSideSlashKnife::Enter(Player* obj)
{

}

void player_state::SecondSideSlashKnife::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::SecondSideSlashKnife::ChangeState(const Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command = CommandHandler::GetInstance();

	// ƒiƒCƒt‘•”õó‘Ô
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<EquipKnife, Player>();
	}

	return nullptr;
}
