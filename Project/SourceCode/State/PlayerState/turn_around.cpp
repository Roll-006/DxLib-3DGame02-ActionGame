#include "turn_around.hpp"

player_state::TurnAround::TurnAround(Player& player) :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kTurnAround)),
	m_is_stop_all_state	(false),
	m_player			(player)
{

}

player_state::TurnAround::~TurnAround()
{

}

void player_state::TurnAround::Update()
{

}

void player_state::TurnAround::LateUpdate()
{

}

void player_state::TurnAround::Enter()
{

}

void player_state::TurnAround::Exit()
{

}

int player_state::TurnAround::GetNextStateKind()
{
	if (m_player.GetDeltaTime() <= 0.0f) { return static_cast<int>(player_state::ActionStateKind::kNone); }

	const auto state_controller = m_player.GetStateController();
	auto next_state_kind = player_state::ActionStateKind::kNone;

	// 死亡
	if (state_controller->TryDead())
	{
		next_state_kind = player_state::ActionStateKind::kDead;
	}
	// メレー(正面蹴り)
	else if (state_controller->TryFrontKick())
	{
		next_state_kind = player_state::ActionStateKind::kFrontKick;
	}
	// メレー(回し蹴り)
	else if (state_controller->TryRoundhouseKick())
	{
		next_state_kind = player_state::ActionStateKind::kRoundhouseKick;
	}
	// ステルスキル
	else if (state_controller->TryStealthKill())
	{
		next_state_kind = player_state::ActionStateKind::kStealthKill;
	}
	// 捕まれる
	else if (state_controller->TryGrabbed())
	{
		next_state_kind = player_state::ActionStateKind::kGrabbed;
	}

	return static_cast<int>(next_state_kind);
}
