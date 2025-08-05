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
	obj->SetLookDirOffsetValueForAim();
	obj->DirOfCameraForward();

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::AimGun::LateUpdate(Player* obj)
{
	const auto gun		= std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());
	const auto camera	= ObjManager::GetInstance()->GetObj<ObjBase>(ObjName.MAIN_CAMERA);

	obj->GetCurrentHeldWeapon()->LateUpdate();

	gun->CalcDiffusionRange();
	gun->CalcTargetPos();
	gun->SetAimDir  (camera->GetTransform()->GetForward	(CoordinateKind::kWorld));
	gun->SetPosOnRay(camera->GetTransform()->GetPos		(CoordinateKind::kWorld));
}

void player_state::AimGun::Enter(Player* obj)
{
	obj->DetachWeapon(obj->GetCurrentEquipWeapon());
	obj->HoldWeapon(obj->GetCurrentEquipWeapon());
}

void player_state::AimGun::Exit(Player* obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon());
}

std::shared_ptr<IState<Player>> player_state::AimGun::ChangeState(Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();
	const auto gun				= std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());

	// 銃装備状態
	if (!command->IsExecuting(CommandKind::kAimGun))
	{
		return state_controller->GetState<EquipGun, Player>();
	}
	// ショット
	if (state_controller->TryPullTrigger(obj))
	{
		if (gun->IsShot())
		{
			return state_controller->GetState<Shot, Player>();
		}
	}
	// リロード
	if (state_controller->TryReload(obj))
	{
		return state_controller->GetState<Reload, Player>();
	}

	return nullptr;
}
