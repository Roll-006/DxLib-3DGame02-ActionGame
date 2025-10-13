#include "grab.hpp"
#include "../../Object/player.hpp"

zombie_state::Grab::Grab() :
	ActionStateBase				(static_cast<int>(zombie_state::ActionStateKind::kGrab)),
	m_grab_camera_controller	(nullptr),
	m_damage_interval_timer		(0.0f),
	m_grab_timer				(0.0f),
	m_is_stop_all_state			(false)
{

}

zombie_state::Grab::~Grab()
{

}

void zombie_state::Grab::Update(std::shared_ptr<Zombie>& obj)
{
	const auto delta_time		= obj->GetDeltaTime();

	m_damage_over_time_start_timer	+= delta_time;
	m_grab_timer					+= delta_time;

	DamageOverTime(obj);
}

void zombie_state::Grab::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Grab::Enter(std::shared_ptr<Zombie>& obj)
{
	m_damage_over_time_start_timer	= 0.0f;
	m_damage_interval_timer			= 0.0f;
	m_grab_timer					= 0.0f;

	// 演出用カメラを生成
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	m_grab_camera_controller = std::make_shared<GrabVirtualCameraController>();
	cinemachine_brain->AddVirtualCameraController(m_grab_camera_controller);

	obj->Grab();
}

void zombie_state::Grab::Exit(std::shared_ptr<Zombie>& obj)
{
	// 演出用カメラを削除
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCameraController(m_grab_camera_controller);
	m_grab_camera_controller = nullptr;

	obj->Release();
}

std::shared_ptr<IState<Zombie>> zombie_state::Grab::ChangeState(std::shared_ptr<Zombie>& obj)
{
	const auto state_controller = obj->GetStateController();

	// 強制NULL
	if (state_controller->TryActionNullForcibly(obj))
	{
		return state_controller->GetState<ActionNull, Zombie>();
	}
	// ノックバック
	if (state_controller->TryKnockback(obj))
	{
		return state_controller->GetState<Knockback, Zombie>();
	}
	// 死亡
	if (state_controller->TryDead(obj))
	{
		return state_controller->GetState<Dead, Zombie>();
	}
	// 左足ダウン
	if (state_controller->TryLeftCrouchStun(obj))
	{
		return state_controller->GetState<CrouchLeftStun, Zombie>();
	}
	// 右足ダウン
	if (state_controller->TryRightCrouchStun(obj))
	{
		return state_controller->GetState<CrouchRightStun, Zombie>();
	}
	// 立ちダウン
	if (state_controller->TryStandStun(obj))
	{
		return state_controller->GetState<StandStun, Zombie>();
	}
	// ノックバック(後ろ)
	if (obj->IsTargetEscaped())
	{
		obj->OnKnockback(-obj->GetCurrentLookDir(), 70.0f, 60.0f);
		return state_controller->GetState<BackwardKnockback, Zombie>();
	}
	// NULL
	if (m_grab_timer > kMaxGrabTime)
	{
		return state_controller->GetState<ActionNull, Zombie>();
	}

	return nullptr;
}

void zombie_state::Grab::DamageOverTime(std::shared_ptr<Zombie>& obj)
{
	if (m_damage_over_time_start_timer < kDamageOverTimeStartTime) { return; }

	const auto state_controller = obj->GetStateController();
	const auto player			= std::dynamic_pointer_cast<Player>(state_controller->GetTargetCharacter());

	// ダメージ処理
	math::Increase(m_damage_interval_timer, obj->GetDeltaTime(), kDamageIntervalTime, true);
	if (m_damage_interval_timer == 0.0f)
	{
		// TODO : 攻撃力からダメージ量を計算
		player->OnDamage(HealthPartKind::kMain, 50.0f);
	}
}
