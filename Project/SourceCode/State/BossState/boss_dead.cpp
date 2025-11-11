#include "boss_dead.hpp"

boss_state::Dead::Dead() :
	ActionStateBase				(static_cast<int>(boss_state::ActionStateKind::kDead)),
	m_is_stop_all_state			(false),
	m_elapsed_time				(0.0f),
	m_dead_cameras_controller	(nullptr)
{

}

boss_state::Dead::~Dead()
{

}

void boss_state::Dead::Update(std::shared_ptr<Boss>& obj)
{
	const auto delta_time = obj->GetDeltaTime();

	obj->DisallowStealthKill();
}

void boss_state::Dead::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Dead::Enter(std::shared_ptr<Boss>& obj)
{
	m_elapsed_time = 0.0f;

	// 演出用カメラを生成
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	m_dead_cameras_controller	 = std::make_shared<GameClearVirtualCamerasController>(obj->GetModeler()->GetModelHandle(), obj->GetTransform());
	cinemachine_brain->AddVirtualCameraController(m_dead_cameras_controller);

	obj->RemoveCollider(ColliderKind::kCollider);

	// 死亡したことを通知
	const DeadBossEvent event{ obj->GetEnemyID(), obj->GetModeler()->GetModelHandle()};
	EventSystem::GetInstance()->Publish(event);
}

void boss_state::Dead::Exit(std::shared_ptr<Boss>& obj)
{
	// 演出用カメラを削除
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCameraController(m_dead_cameras_controller);
	m_dead_cameras_controller = nullptr;
}

std::shared_ptr<IState<Boss>> boss_state::Dead::ChangeState(std::shared_ptr<Boss>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	return nullptr;
}
