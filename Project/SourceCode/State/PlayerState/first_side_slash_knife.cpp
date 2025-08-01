#include "first_side_slash_knife.hpp"

player_state::FirstSideSlashKnife::FirstSideSlashKnife() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kFirstSideSlashKnife))
{

}

player_state::FirstSideSlashKnife::~FirstSideSlashKnife()
{

}

void player_state::FirstSideSlashKnife::Update(Player* obj)
{

}

void player_state::FirstSideSlashKnife::LateUpdate(Player* obj)
{

}

void player_state::FirstSideSlashKnife::Enter(Player* obj)
{

}

void player_state::FirstSideSlashKnife::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::FirstSideSlashKnife::ChangeState(const Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	// 切り裂く(第二段階)
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody) && command->IsExecuting(CommandKind::kAttack))
	{
		return state_controller->GetState<SecondSideSlashKnife, Player>();
	}
	// ナイフ装備状態
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<EquipKnife, Player>();
	}

	return nullptr;
}
