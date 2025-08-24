#pragma once
#include "../../Base/move_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"
#include "../../Command/command_handler.hpp"

namespace player_state
{
	class Move final : public MoveStateBase<Player>
	{
	public:
		Move();
		~Move() override;

		void Update		(std::shared_ptr<Player> obj) override;
		void LateUpdate	(std::shared_ptr<Player> obj) override;
		void Enter		(std::shared_ptr<Player> obj) override;
		void Exit		(std::shared_ptr<Player> obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(std::shared_ptr<Player> obj) override;

	private:
		bool m_is_first_move_frame;		// 移動を開始して最初のフレームであるかを判定
	};
}
