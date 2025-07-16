#pragma once
#include "../../Base/move_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"
#include "../../Manager/command_handler.hpp"

namespace player_state
{
	class Move final : public MoveStateBase<Player>
	{
	public:
		Move();
		~Move() override;

		void Update(Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(const Player* obj) override;

	private:

	};
}
