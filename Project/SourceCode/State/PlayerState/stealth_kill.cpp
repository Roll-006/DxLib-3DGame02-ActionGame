#include "stealth_kill.hpp"

player_state::StealthKill::StealthKill() :
	ActionStateBase					(static_cast<int>(player_state::ActionStateKind::kStealthKill)),
	m_is_stop_all_state				(true),
	m_stealth_kill_camera_controller(nullptr)
{

}

player_state::StealthKill::~StealthKill()
{

}

void player_state::StealthKill::Update(std::shared_ptr<Player>& obj)
{
	obj->UpdateStealthKill();
}

void player_state::StealthKill::LateUpdate(std::shared_ptr<Player>& obj)
{
	obj->OnFootIK();
}

void player_state::StealthKill::Enter(std::shared_ptr<Player>& obj)
{
	// 演出用カメラを生成
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	m_stealth_kill_camera_controller = std::make_shared<StealthKillVirtualCameraController>();
	cinemachine_brain->AddVirtualCameraController(m_stealth_kill_camera_controller);

	obj->SetupStealthKill();
}

void player_state::StealthKill::Exit(std::shared_ptr<Player>& obj)
{
	// 演出用カメラを削除
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCameraController(m_stealth_kill_camera_controller);
	m_stealth_kill_camera_controller = nullptr;

	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

std::shared_ptr<IState<Player>> player_state::StealthKill::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// NULL
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<ActionNull, Player>();
	}

	return nullptr;
}
