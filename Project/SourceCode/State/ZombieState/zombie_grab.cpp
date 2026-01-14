#include "../../Object/zombie.hpp"
#include "../../Kind/zombie_anim_kind.hpp"
#include "../../Object/player.hpp"
#include "../../Animator/animator.hpp"
#include "../../Kind/zombie_state_kind.hpp"
#include "../../VirtualCamera/cinemachine_brain.hpp"
#include "zombie_state.hpp"
#include "zombie_grab.hpp"

zombie_state::Grab::Grab(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator) :
	ZombieStateBase(zombie, state, animator, ZombieStateKind::kGrab),
	m_grab_camera_controller(nullptr),
	m_damage_over_time_start_timer(0.0f),
	m_damage_interval_timer(0.0f),
	m_grab_timer(0.0f)
{

}

zombie_state::Grab::~Grab()
{

}

void zombie_state::Grab::Update()
{
	m_zombie.CalcMoveSpeedStop();
	m_zombie.InitMoveOffset();

	const float delta_time = m_zombie.GetDeltaTime();

	m_damage_over_time_start_timer += delta_time;
	m_grab_timer += delta_time;

	DamageOverTime();

	m_zombie.UpdateLocomotion();

	m_animator->AttachResultAnim(static_cast<int>(ZombieAnimKind::kEnterNeckBite));
}

void zombie_state::Grab::LateUpdate()
{
	m_zombie.OnFootIK();
}

void zombie_state::Grab::Enter()
{
	m_damage_over_time_start_timer = 0.0f;
	m_damage_interval_timer = 0.0f;
	m_grab_timer = 0.0f;

	// 演出用カメラ生成
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	m_grab_camera_controller = std::make_shared<GrabVirtualCameraController>();
	cinemachine_brain->AddVirtualCameraController(m_grab_camera_controller);

	m_zombie.Grab();
}

void zombie_state::Grab::Exit()
{
	// 演出用カメラ削除
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCameraController(m_grab_camera_controller);
	m_grab_camera_controller = nullptr;

	m_zombie.Release();
}

const ZombieStateKind zombie_state::Grab::GetNextStateKind()
{
	if (m_zombie.GetDeltaTime() <= 0.0f)
	{
		return ZombieStateKind::kNone;
	}
	// 強制待機
	else if (m_state.TryWaitForcibly())
	{
		return ZombieStateKind::kIdle;
	}
	// 対象が逃げた
	else if (m_zombie.IsTargetEscaped())
	{
		m_zombie.OnKnockback(-m_zombie.GetLookDir(TimeKind::kCurrent), 70.0f, 60.0f);
		return ZombieStateKind::kBackwardKnockback;
	}
	// 掴み時間終了
	else if (m_grab_timer > kMaxGrabTime)
	{
		return ZombieStateKind::kIdle;
	}
	//// ステルスキル
	//else if (m_state.TryStealthKilled())
	//{
	//	return ZombieStateKind::kStealthKilled;
	//}
	//// ノックバック
	//else if (m_state.TryKnockback())
	//{
	//	return ZombieStateKind::kKnockback;
	//}
	//// ノックバック（後ろ）
	//else if (m_state.TryBackwardKnockback())
	//{
	//	// TODO : State遷移側で処理したい
	//	m_zombie.OnKnockback(-m_zombie.GetCurrentLookDir(), 70.0f, 60.0f);
	//	return ZombieStateKind::kBackwardKnockback;
	//}
	//// 死亡
	//else if (m_state.TryDead())
	//{
	//	return ZombieStateKind::kDead;
	//}
	//// 左足ダウン
	//else if (m_state.TryLeftCrouchStun())
	//{
	//	return ZombieStateKind::kCrouchLeftStun;
	//}
	//// 右足ダウン
	//else if (m_state.TryRightCrouchStun())
	//{
	//	return ZombieStateKind::kCrouchRightStun;
	//}
	//// 立ちダウン
	//else if (m_state.TryStandStun())
	//{
	//	return ZombieStateKind::kStandStun;
	//}

	return ZombieStateKind::kNone;
}

void zombie_state::Grab::DamageOverTime()
{
	if (m_damage_over_time_start_timer < kDamageOverTimeStartTime)
	{
		return;
	}

	//const auto player = std::dynamic_pointer_cast<Player>(m_state.GetTargetCharacter());
	//if (!player)
	//{
	//	return;
	//}

	// 継続ダメージ
	math::Increase(m_damage_interval_timer, m_zombie.GetDeltaTime(), kDamageIntervalTime, true);
	if (m_damage_interval_timer == 0.0f)
	{
		// TODO : 攻撃力ベースにする
		m_zombie.GetTarget()->OnDamage(HealthPartKind::kMain, 50.0f);
	}
}
