#pragma once
#include "../../Interface/i_action_state.hpp"
#include "../../Object/player.hpp"

namespace player_state
{
	class TurnAround final : public IActionState<Player>
	{
	public:
		TurnAround();
		~TurnAround() override;

		void Update(const Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(const Player* obj) override;

	private:

	};
}
