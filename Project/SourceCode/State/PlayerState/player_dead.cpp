#include "player_dead.hpp"

player_state::Dead::Dead(Player& player) :
	ActionStateBase				(static_cast<int>(player_state::ActionStateKind::kDead)),
	m_is_stop_all_state			(false),
	m_elapsed_time				(0.0f),
	m_is_seted_time_scale		(false),
	m_dead_cameras_controller	(nullptr),
	m_player					(player)
{

}

player_state::Dead::~Dead()
{
	if (m_dead_cameras_controller)
	{
		// 演出用カメラを削除
		const auto cinemachine_brain = CinemachineBrain::GetInstance();
		cinemachine_brain->RemoveVirtualCameraController(m_dead_cameras_controller);
		m_dead_cameras_controller = nullptr;
	}
}

void player_state::Dead::Update()
{
	const auto game_time_manager	= GameTimeManager::GetInstance();
	const auto delta_time			= game_time_manager->GetDeltaTime(TimeScaleLayerKind::kNoneScale);
	m_elapsed_time += delta_time;

	if (m_elapsed_time > 6.5f && !m_is_seted_time_scale)
	{
		m_is_seted_time_scale = true;

		game_time_manager->SetTimeScale(TimeScaleLayerKind::kWorld,  0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kPlayer, 0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kEffect, 0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kCamera, 0.0f);
	}
}

void player_state::Dead::LateUpdate()
{

}

void player_state::Dead::Enter()
{
	m_elapsed_time			= 0.0f;
	m_is_seted_time_scale	= false;

	const auto cinemachine_brain	= CinemachineBrain::GetInstance();
	m_dead_cameras_controller		= std::make_shared<DeadVirtualCamerasController>();
	cinemachine_brain->AddVirtualCameraController(m_dead_cameras_controller);

	const DeadPlayerEvent event{ obj->GetModeler()->GetModelHandle() };
	EventSystem::GetInstance()->Publish(event);
}

void player_state::Dead::Exit()
{
	// 演出用カメラを削除
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCameraController(m_dead_cameras_controller);
	m_dead_cameras_controller = nullptr;
}

int player_state::Dead::GetNextStateKind()
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	// 死亡からの遷移はない
	return nullptr;
}
