#pragma once
#include "../../Base/move_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"
#include "../../Command/command_handler.hpp"

namespace player_state
{
	class Move final : public MoveStateBase
	{
	public:
		Move(Player& player);
		~Move() override;

		void Update() override;
		void LateUpdate() override;
		void Enter() override;
		void Exit() override;

		[[nodiscard]] int GetNextStateKind() override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		bool m_is_first_move_frame;		// 移動を開始して最初のフレームであるかを判定
		bool m_is_stop_all_state;
		Player& m_player;
	};
}
