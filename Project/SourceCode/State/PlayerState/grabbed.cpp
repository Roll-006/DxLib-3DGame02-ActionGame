#include "grabbed.hpp"

player_state::Grabbed::Grabbed() :
	ActionStateBase			(static_cast<int>(player_state::ActionStateKind::kGrabbed)),
	m_subject				(std::make_shared<Subject<Grabbed>>()),
	m_grab_camera_controller(nullptr)
{

}

player_state::Grabbed::~Grabbed()
{

}

void player_state::Grabbed::Update(std::shared_ptr<Player> obj)
{
	obj->UpdateGrabbed();
}

void player_state::Grabbed::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::Grabbed::Enter(std::shared_ptr<Player> obj)
{
	const auto chnemachine_brain = CinemachineBrain::GetInstance();
	m_grab_camera_controller = std::dynamic_pointer_cast<GrabVirtualCameraController>(chnemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kGrabCutscene));
	m_subject->AddObserver(m_grab_camera_controller);

	// ’Í‚Ü‚ê‚½‚±‚Æ‚ğ‰‰oƒJƒƒ‰‚É’Ê’m
	const Event<GrabbedData> event = { EventKind::kGrabbed, { obj->GetModeler()->GetModelHandle() }};
	m_subject->Notify(event);
}

void player_state::Grabbed::Exit(std::shared_ptr<Player> obj)
{
	m_subject->RemoveObserver(m_grab_camera_controller);
	m_grab_camera_controller = nullptr;
}

std::shared_ptr<IState<Player>> player_state::Grabbed::ChangeState(std::shared_ptr<Player> obj)
{
	const auto state_controller = obj->GetStateController();

	// —£‚³‚ê‚é
	if (!state_controller->TryGrabbed(obj))
	{
		return state_controller->GetState<ActionNull, Player>();
	}

	return nullptr;
}
