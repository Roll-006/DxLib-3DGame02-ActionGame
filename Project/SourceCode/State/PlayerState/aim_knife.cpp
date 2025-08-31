#include "aim_knife.hpp"

player_state::AimKnife::AimKnife() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kAimKnife))
{

}

player_state::AimKnife::~AimKnife()
{

}

void player_state::AimKnife::Update(std::shared_ptr<Player> obj)
{
	obj->SetLookDirOffsetValueForAim();
	obj->DirOfCameraForward();

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::AimKnife::LateUpdate(std::shared_ptr<Player> obj)
{
	const auto camera	= ObjManager::GetInstance()->GetObj<ObjBase>(ObjName.MAIN_CAMERA);
	const auto aim_dir	= camera->GetTransform()->GetForward(CoordinateKind::kWorld);

	// ボーン位置補正
	obj->GetBonePosCorrector()->CorrectAimPoseBonePos(obj->GetModeler()->GetModelHandle(), aim_dir);

	obj->GetCurrentHeldWeapon()->TrackOwnerHand();
}

void player_state::AimKnife::Enter(std::shared_ptr<Player> obj)
{
	obj->DetachWeapon(obj->GetCurrentEquipKnife());
	obj->HoldWeapon(obj->GetCurrentEquipKnife());
}

void player_state::AimKnife::Exit(std::shared_ptr<Player> obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipKnife());
}

std::shared_ptr<IState<Player>> player_state::AimKnife::ChangeState(std::shared_ptr<Player> obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();
	const auto weapon_kind		= obj->GetCurrentHeldWeaponKind();

	// 銃装備状態
	if (weapon_kind == WeaponKind::kGun)
	{
		return state_controller->GetState<EquipGun, Player>();
	}
	// ナイフ装備状態
	if (!command->IsExecute(CommandKind::kAimKnife, TimeKind::kCurrent))
	{
		return state_controller->GetState<EquipKnife, Player>();
	}
	// 突き刺す
	if (command->IsExecute(CommandKind::kAttack, TimeKind::kCurrent)
		|| command->IsExecute(CommandKind::kReload, TimeKind::kCurrent))
	{
		return state_controller->GetState<StabKnife, Player>();
	}

	return nullptr;
}
