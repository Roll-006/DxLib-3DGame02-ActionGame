#include "../Object/player.hpp"
#include "../Animator/animator.hpp"
#include "../State/PlayerState/player_state.hpp"
#include "../Kind/player_state_kind.hpp"
#include "../Kind/player_anim_kind.hpp"
#include "../Command/command_handler.hpp"
#include "player_state_base.hpp"

PlayerStateBase::PlayerStateBase(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator, const PlayerStateKind state_kind) :
	m_player						(player),
	m_state							(state),
	m_animator						(animator),
	m_state_kind					(state_kind),
	m_basic_anim_kind				{ {Animator::BodyKind::kLowerBody, PlayerAnimKind::kIdle},				 {Animator::BodyKind::kUpperBody, PlayerAnimKind::kIdle} },
	m_walk_forward_anim_kind		{ {Animator::BodyKind::kLowerBody, PlayerAnimKind::kMoveForward},		 {Animator::BodyKind::kUpperBody, PlayerAnimKind::kMoveForward} },
	m_run_forward_anim_kind			{ {Animator::BodyKind::kLowerBody, PlayerAnimKind::kMoveForwardRun},     {Animator::BodyKind::kUpperBody, PlayerAnimKind::kMoveForwardRun} },
	m_crouch_walk_forward_anim_kind	{ {Animator::BodyKind::kLowerBody, PlayerAnimKind::kMoveForwardCrouch},	 {Animator::BodyKind::kUpperBody, PlayerAnimKind::kMoveForwardCrouch} },
	m_crouch_anim_kind				{ {Animator::BodyKind::kLowerBody, PlayerAnimKind::kCrouch},			 {Animator::BodyKind::kUpperBody, PlayerAnimKind::kCrouch} },
	m_is_crouch						(false),
	m_is_run						(false),
	m_prev_crouch					(false),
	m_prev_run						(false)
{
}

PlayerStateBase::~PlayerStateBase()
{

}

void PlayerStateBase::Move()
{
	if (TryMove())
	{
		UpdateMove();
		m_player.CalcMoveSpeed();
		m_player.DirOfCameraForward();

		const auto command			= CommandHandler::GetInstance();
		const auto is_move_forward	= command->IsExecute(CommandKind::kMoveUpPlayer,	TimeKind::kCurrent);
		const auto is_move_backward = command->IsExecute(CommandKind::kMoveDownPlayer,	TimeKind::kCurrent);
		const auto is_move_left		= command->IsExecute(CommandKind::kMoveLeftPlayer,	TimeKind::kCurrent);
		const auto is_move_right	= command->IsExecute(CommandKind::kMoveRightPlayer, TimeKind::kCurrent);

		// アニメーションアタッチ
		m_animator->AttachAnimEightDir(static_cast<int>(m_walk_forward_anim_kind.at(Animator::BodyKind::kLowerBody)), is_move_forward, is_move_backward, is_move_left, is_move_right, false);
		m_animator->AttachAnim		  (static_cast<int>(m_walk_forward_anim_kind.at(Animator::BodyKind::kUpperBody)), Animator::BodyKind::kUpperBody);
	}
	else
	{
		m_player.CalcMoveSpeedStop();

		// アニメーションアタッチ
		if (m_basic_anim_kind.at(Animator::BodyKind::kLowerBody) == m_basic_anim_kind.at(Animator::BodyKind::kUpperBody))
		{
			m_animator->AttachResultAnim(static_cast<int>(m_basic_anim_kind.at(Animator::BodyKind::kLowerBody)));
		}
		else
		{
			m_animator->AttachAnim(static_cast<int>(m_basic_anim_kind.at(Animator::BodyKind::kLowerBody)), Animator::BodyKind::kLowerBody);
			m_animator->AttachAnim(static_cast<int>(m_basic_anim_kind.at(Animator::BodyKind::kUpperBody)), Animator::BodyKind::kUpperBody);
		}
	}

	const auto command = CommandHandler::GetInstance();
	command->InitCurrentTriggerInputCount(CommandKind::kCrouch);
	command->InitCurrentTriggerInputCount(CommandKind::kRun);
}

void PlayerStateBase::BasicMove()
{
	m_is_crouch = false;
	m_is_run	= false;

	if (TryMove())
	{
		UpdateMove();
		const auto command			= CommandHandler::GetInstance();
		const auto is_move_forward	= command->IsExecute(CommandKind::kMoveUpPlayer,	TimeKind::kCurrent);
		const auto is_move_backward = command->IsExecute(CommandKind::kMoveDownPlayer,	TimeKind::kCurrent);
		const auto is_move_left		= command->IsExecute(CommandKind::kMoveLeftPlayer,	TimeKind::kCurrent);
		const auto is_move_right	= command->IsExecute(CommandKind::kMoveRightPlayer, TimeKind::kCurrent);

		if (TryRun())
		{
			m_is_run = true;

			UpdateRun();

			// アニメーションアタッチ
			if (m_run_forward_anim_kind.at(Animator::BodyKind::kLowerBody) == m_run_forward_anim_kind.at(Animator::BodyKind::kUpperBody))
			{
				m_animator->AttachResultAnim(static_cast<int>(m_run_forward_anim_kind.at(Animator::BodyKind::kLowerBody)));
			}
			else
			{
				m_animator->AttachAnim(static_cast<int>(m_run_forward_anim_kind.at(Animator::BodyKind::kLowerBody)), Animator::BodyKind::kLowerBody);
				m_animator->AttachAnim(static_cast<int>(m_run_forward_anim_kind.at(Animator::BodyKind::kUpperBody)), Animator::BodyKind::kUpperBody);
			}
		}
		else if (TryCrouch())
		{
			m_is_crouch = true;

			UpdateCrouch();
			m_player.CalcMoveSpeed();

			// アニメーションアタッチ
			m_animator->AttachAnimEightDir(static_cast<int>(m_crouch_walk_forward_anim_kind.at(Animator::BodyKind::kLowerBody)), is_move_forward, is_move_backward, is_move_left, is_move_right, false);
			m_animator->AttachAnim		  (static_cast<int>(m_crouch_walk_forward_anim_kind.at(Animator::BodyKind::kUpperBody)), Animator::BodyKind::kUpperBody);
		}
		else
		{
			m_player.CalcMoveSpeed();
			m_player.DirOfCameraForward();

			// アニメーションアタッチ
			m_animator->AttachAnimEightDir(static_cast<int>(m_walk_forward_anim_kind.at(Animator::BodyKind::kLowerBody)), is_move_forward, is_move_backward, is_move_left, is_move_right, false);
			m_animator->AttachAnim		  (static_cast<int>(m_walk_forward_anim_kind.at(Animator::BodyKind::kUpperBody)), Animator::BodyKind::kUpperBody);
		}
	}
	else if (TryCrouch())
	{
		m_is_crouch = true;

		UpdateCrouch();
		m_player.CalcMoveSpeedStop();

		// アニメーションアタッチ
		if (m_crouch_anim_kind.at(Animator::BodyKind::kLowerBody) == m_crouch_anim_kind.at(Animator::BodyKind::kUpperBody))
		{
			m_animator->AttachResultAnim(static_cast<int>(m_crouch_anim_kind.at(Animator::BodyKind::kLowerBody)));
		}
		else
		{
			m_animator->AttachAnim(static_cast<int>(m_crouch_anim_kind.at(Animator::BodyKind::kLowerBody)), Animator::BodyKind::kLowerBody);
			m_animator->AttachAnim(static_cast<int>(m_crouch_anim_kind.at(Animator::BodyKind::kUpperBody)), Animator::BodyKind::kUpperBody);
		}
	}
	else
	{
		m_player.CalcMoveSpeedStop();

		// アニメーションアタッチ
		if (m_basic_anim_kind.at(Animator::BodyKind::kLowerBody) == m_basic_anim_kind.at(Animator::BodyKind::kUpperBody))
		{
			m_animator->AttachResultAnim(static_cast<int>(m_basic_anim_kind.at(Animator::BodyKind::kLowerBody)));
		}
		else
		{
			m_animator->AttachAnim(static_cast<int>(m_basic_anim_kind.at(Animator::BodyKind::kLowerBody)), Animator::BodyKind::kLowerBody);
			m_animator->AttachAnim(static_cast<int>(m_basic_anim_kind.at(Animator::BodyKind::kUpperBody)), Animator::BodyKind::kUpperBody);
		}
	}

	const auto command = CommandHandler::GetInstance();
	if(!m_is_crouch && m_prev_crouch)	{ command->InitCurrentTriggerInputCount(CommandKind::kCrouch); }
	if(!m_is_run	&& m_prev_run)		{ command->InitCurrentTriggerInputCount(CommandKind::kRun); }

	m_prev_crouch	= m_is_crouch;
	m_prev_run		= m_is_run;
}


#pragma region 汎用Try判定
const bool PlayerStateBase::TryMove() const
{
	if (!m_player.CanControl()) { return false; }

	const auto command = CommandHandler::GetInstance();

	return((command->IsExecute(CommandKind::kMoveUpPlayer,	  TimeKind::kCurrent)
		 || command->IsExecute(CommandKind::kMoveDownPlayer,  TimeKind::kCurrent)
		 || command->IsExecute(CommandKind::kMoveLeftPlayer,  TimeKind::kCurrent)
		 || command->IsExecute(CommandKind::kMoveRightPlayer, TimeKind::kCurrent)));
}

const bool PlayerStateBase::TryCrouch() const
{
	if (!m_player.CanControl()) { return false; }

	return CommandHandler::GetInstance()->IsExecute(CommandKind::kCrouch, TimeKind::kCurrent);
}

const bool PlayerStateBase::TryRun() const
{
	if (!m_player.CanControl()) { return false; }

	const auto command = CommandHandler::GetInstance();

	if (TryMove())
	{
		if (command->IsExecute(CommandKind::kRun, TimeKind::kCurrent))
		{
			return true;
		}
	}
	// IDLEであった場合、次フレームのためにトリガーカウントを増やしておく
	else
	{
		// 現在の入力方式を保存する
		const bool is_trigger = command->GetInputModeKind(CommandKind::kRun) == InputModeKind::kTrigger;

		// 入力モードを強制的にホールド方式に変更
		command->SetInputMode(CommandKind::kRun, InputModeKind::kHold);

		// もともとトリガー方式であれば元に戻す
		if (is_trigger) { command->SetInputMode(CommandKind::kRun, InputModeKind::kTrigger); }
	}

	return false;
}
#pragma endregion


#pragma region 汎用行動
void PlayerStateBase::UpdateMove()
{
	m_player.Move();
}

void  PlayerStateBase::UpdateCrouch()
{
	m_player.DirOfCameraForward();
}

void  PlayerStateBase::UpdateRun()
{
	m_player.CalcMoveSpeedRun();
	m_player.DirOfMovement();
}
#pragma endregion
