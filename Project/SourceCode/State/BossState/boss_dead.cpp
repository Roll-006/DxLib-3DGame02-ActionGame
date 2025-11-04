#include "boss_dead.hpp"

boss_state::Dead::Dead() :
	ActionStateBase				(static_cast<int>(boss_state::ActionStateKind::kDead)),
	m_is_stop_all_state			(false),
	m_elapsed_time				(0.0f),
	m_is_seted_time_scale		(false),
	m_change_color_wait_time	(0.0f),
	m_disappear_wait_time		(0.0f),
	m_is_disappear_notified		(false),
	m_current_material			(),
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

	// タイムスケールを変更
	m_elapsed_time += delta_time;
	if (m_elapsed_time > 6.5f && !m_is_seted_time_scale)
	{
		m_is_seted_time_scale = true;

		const auto game_time_manager = GameTimeManager::GetInstance();
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kWorld,  0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kPlayer, 0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kEffect, 0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kCamera, 0.0f);
	}

	// 色を黒に変化
	m_change_color_wait_time += delta_time;
	if (m_change_color_wait_time >= kChangeColorWaitTime)
	{
		ChangeMaterial(obj->GetModeler()->GetModelHandle(), 1.0f * delta_time);
	}

	// ボスを消す
	m_disappear_wait_time += delta_time;
	if (m_disappear_wait_time >= kDisappearWaitTime && !m_is_disappear_notified)
	{
		obj->AllowReturnPool();

		const DisappearBossEvent event{ obj->GetModeler()->GetModelHandle() };
		EventSystem::GetInstance()->Publish(event);

		m_is_disappear_notified = true;
	}
}

void boss_state::Dead::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Dead::Enter(std::shared_ptr<Boss>& obj)
{
	m_elapsed_time				= 0.0f;
	m_is_seted_time_scale		= false;
	m_change_color_wait_time	= 0.0f;
	m_disappear_wait_time		= 0.0f;
	m_is_disappear_notified		= false;
	m_current_material			= MaterialData();

	// 演出用カメラを生成
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	m_dead_cameras_controller	 = std::make_shared<DeadBossVirtualCamerasController>(obj->GetModeler()->GetModelHandle(), obj->GetTransform());
	cinemachine_brain->AddVirtualCameraController(m_dead_cameras_controller);

	obj->RemoveCollider(ColliderKind::kCollider);

	// 死亡したことを通知
	const DeadBossEvent event{ obj->GetEnemyID(), obj->GetModeler()->GetModelHandle()};
	EventSystem::GetInstance()->Publish(event);

	const auto game_time_manager = GameTimeManager::GetInstance();
	game_time_manager->SetTimeScale(TimeScaleLayerKind::kPlayer, 0.0f);
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

void boss_state::Dead::ChangeMaterial(const auto model_handle, const float change_speed)
{
	math::Decrease(m_current_material.diffuse_color.r,  change_speed, 0.0f);
	math::Decrease(m_current_material.diffuse_color.g,  change_speed, 0.0f);
	math::Decrease(m_current_material.diffuse_color.b,  change_speed, 0.0f);

	math::Decrease(m_current_material.specular_color.r, change_speed, 0.0f);
	math::Decrease(m_current_material.specular_color.g, change_speed, 0.0f);
	math::Decrease(m_current_material.specular_color.b, change_speed, 0.0f);

	math::Decrease(m_current_material.emissive_color.r, change_speed, 0.0f);
	math::Decrease(m_current_material.emissive_color.g, change_speed, 0.0f);
	math::Decrease(m_current_material.emissive_color.b, change_speed, 0.0f);

	math::Decrease(m_current_material.ambient_color.r,  change_speed, 0.0f);
	math::Decrease(m_current_material.ambient_color.g,  change_speed, 0.0f);
	math::Decrease(m_current_material.ambient_color.b,  change_speed, 0.0f);

	MV1SetDifColorScale(model_handle, m_current_material.diffuse_color );
	MV1SetSpcColorScale(model_handle, m_current_material.specular_color);
	MV1SetEmiColorScale(model_handle, m_current_material.emissive_color);
	MV1SetAmbColorScale(model_handle, m_current_material.ambient_color );
}
