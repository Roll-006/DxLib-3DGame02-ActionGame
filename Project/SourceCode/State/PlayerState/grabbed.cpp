#include "grabbed.hpp"

player_state::Grabbed::Grabbed() :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kGrabbed)),
	m_is_stop_all_state	(true),
	m_escape_start_timer(0.0f)
{

}

player_state::Grabbed::~Grabbed()
{

}

void player_state::Grabbed::Update(std::shared_ptr<Player>& obj)
{
	obj->UpdateGrabbed();

	Escape(obj);
}

void player_state::Grabbed::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::Grabbed::Enter(std::shared_ptr<Player>& obj)
{
	// 掴まれたことを演出カメラに通知
	const GrabbedEvent event{ obj->GetModeler() };
	EventSystem::GetInstance()->Publish(event);

	m_escape_start_timer = 0.0f;
	obj->GetEscapeGauge()->DecreaseZero();
}

void player_state::Grabbed::Exit(std::shared_ptr<Player>& obj)
{

}

std::shared_ptr<IState<Player>> player_state::Grabbed::ChangeState(std::shared_ptr<Player>& obj)
{
	const auto state_controller = obj->GetStateController();

	// 離される
	if (!state_controller->TryGrabbed(obj))
	{
		return state_controller->GetState<ActionNull, Player>();
	}

	return nullptr;
}

void player_state::Grabbed::Escape(std::shared_ptr<Player>& obj)
{
	m_escape_start_timer += obj->GetDeltaTime();
	if (m_escape_start_timer < kEscapeStartTime) { return; }

	const auto command		= CommandHandler::GetInstance();
	const auto input_mode	= command->GetInputModeKind(CommandKind::kEscape);
	auto	   increase		= 0.0f;
	auto	   escape_gauge = obj->GetEscapeGauge();

	switch (input_mode)
	{
	case InputModeKind::kSingle:
		increase = escape_gauge->GetMaxValue() / kEscapeInputNum;
		break;

	case InputModeKind::kHold:
		increase = escape_gauge->GetMaxValue() / kEscapeMinTime * obj->GetDeltaTime();
		break;

	default:
		break;
	}

	if (command->IsExecute(CommandKind::kEscape, TimeKind::kCurrent))
	{
		escape_gauge->Increase(increase);
	}

	// 脱出
	if (escape_gauge->IsMax())
	{
		obj->GetGrabber()->ReleaseForcibly();
	}
}
