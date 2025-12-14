#include "crouch_turn_around.hpp"

player_state::CrouchTurnAround::CrouchTurnAround(Player& player) :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kCrouchTurnAround)),
	m_is_stop_all_state	(false),
	m_player			(player)
{

}

player_state::CrouchTurnAround::~CrouchTurnAround()
{

}

void player_state::CrouchTurnAround::Update()
{

}

void player_state::CrouchTurnAround::LateUpdate()
{

}

void player_state::CrouchTurnAround::Enter()
{

}

void player_state::CrouchTurnAround::Exit()
{

}

int player_state::CrouchTurnAround::GetNextStateKind()
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// 死亡
	if (state_controller->TryDead(obj))
	{
		return state_controller->GetState<Dead, Player>();
	}
	// メレー(正面蹴り)
	if (state_controller->TryFrontKick(obj))
	{
		return state_controller->GetState<FrontKick, Player>();
	}
	// メレー(回し蹴り)
	if (state_controller->TryRoundhouseKick(obj))
	{
		return state_controller->GetState<RoundhouseKick, Player>();
	}
	// ステルスキル
	if (state_controller->TryStealthKill(obj))
	{
		return state_controller->GetState<StealthKill, Player>();
	}
	// 捕まれる
	if (state_controller->TryGrabbed(obj))
	{
		return state_controller->GetState<Grabbed, Player>();
	}

	return nullptr;
}
