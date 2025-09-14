#include "grab.hpp"
#include "../../Object/player.hpp"

zombie_state::Grab::Grab() :
	ActionStateBase			(static_cast<int>(zombie_state::ActionStateKind::kGrab)),
	m_subject				(std::make_shared<Subject<Grab>>()),
	m_grab_camera_controller(nullptr),
	m_damage_interval_timer	(0.0f)
{

}

zombie_state::Grab::~Grab()
{

}

void zombie_state::Grab::Update(std::shared_ptr<Zombie> obj)
{
	const auto state_controller = obj->GetStateController();
	const auto player			= std::dynamic_pointer_cast<Player>(state_controller->GetTargetCharacter());

	// ダメージ処理
	math::Increase(m_damage_interval_timer, obj->GetDeltaTime(), kDamageIntervalTime, true);
	if (m_damage_interval_timer == 0.0f)
	{
		// TODO : 攻撃力からダメージ量を計算
		player->OnGrabbedDamage(50.0f);
	}
}

void zombie_state::Grab::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Grab::Enter(std::shared_ptr<Zombie> obj)
{
	m_damage_interval_timer = 0.0f;

	// 演出用カメラを生成
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	m_grab_camera_controller = std::make_shared<GrabVirtualCameraController>();
	cinemachine_brain->AddVirtualCameraController(m_grab_camera_controller);
	m_subject->AddObserver(m_grab_camera_controller);

	// 掴んだことを演出カメラに通知
	const Event<GrabData> event = { EventKind::kGrab, { obj->GetModeler()->GetModelHandle() } };
	m_subject->Notify(event);

	// プレイヤーの掴まれた関数を呼び出す
	const auto state_controller = obj->GetStateController();
	const auto player			= std::dynamic_pointer_cast<Player>(state_controller->GetTargetCharacter());
	if (player)
	{
		player->OnGrabbed(obj->GetTransform()->GetPos(CoordinateKind::kWorld), obj->GetCurrentLookDir());
	}

	obj->SetAttackIntervalTime();
}

void zombie_state::Grab::Exit(std::shared_ptr<Zombie> obj)
{
	// 演出用カメラを削除
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCameraController(m_grab_camera_controller);
	m_subject->RemoveObserver(m_grab_camera_controller);
	m_grab_camera_controller = nullptr;

	const auto state_controller = obj->GetStateController();
	const auto player			= std::dynamic_pointer_cast<Player>(state_controller->GetTargetCharacter());
	if (player)
	{
		player->OnRelease();
	}
}

std::shared_ptr<IState<Zombie>> zombie_state::Grab::ChangeState(std::shared_ptr<Zombie> obj)
{
	const auto state_controller = obj->GetStateController();

	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<ActionNull, Zombie>();
	}

	return nullptr;
}
