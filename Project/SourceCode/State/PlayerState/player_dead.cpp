#include "../../Object/player.hpp"
#include "../../Kind/player_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Event/event_system.hpp"
#include "../../Kind/player_state_kind.hpp"
#include "player_state.hpp"
#include "player_dead.hpp"

player_state::Dead::Dead(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator) :
	PlayerStateBase(player, state, animator, PlayerStateKind::kDead),
	m_is_stop_all_state(false),
	m_elapsed_time(0.0f),
	m_is_seted_time_scale(false),
	m_dead_cameras_controller(nullptr)
{
}

player_state::Dead::~Dead()
{
	if (m_dead_cameras_controller)
	{
		const auto cinemachine_brain = CinemachineBrain::GetInstance();
		cinemachine_brain->RemoveVirtualCameraController(m_dead_cameras_controller);
		m_dead_cameras_controller = nullptr;
	}
}

void player_state::Dead::Update()
{
	m_animator->AttachResultAnim(static_cast<int>(PlayerAnimKind::kDead));

	const auto game_time_manager = GameTimeManager::GetInstance();
	const auto delta_time = game_time_manager->GetDeltaTime(TimeScaleLayerKind::kNoneScale);
	m_elapsed_time += delta_time;

	if (m_elapsed_time > 6.5f && !m_is_seted_time_scale)
	{
		m_is_seted_time_scale = true;
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kWorld,  0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kPlayer, 0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kEffect, 0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kCamera, 0.0f);
	}

	m_player.CalcMoveSpeedStop();
}

void player_state::Dead::LateUpdate()
{
}

void player_state::Dead::Enter()
{
	m_elapsed_time = 0.0f;
	m_is_seted_time_scale = false;

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	m_dead_cameras_controller = std::make_shared<DeadVirtualCamerasController>();
	cinemachine_brain->AddVirtualCameraController(m_dead_cameras_controller);

	const DeadPlayerEvent event{ m_player.GetModeler()->GetModelHandle() };
	EventSystem::GetInstance()->Publish(event);
}

void player_state::Dead::Exit()
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCameraController(m_dead_cameras_controller);
	m_dead_cameras_controller = nullptr;
}

const PlayerStateKind player_state::Dead::GetNextStateKind()
{
	// 死亡からの遷移はなし
	return PlayerStateKind::kNone;
}
