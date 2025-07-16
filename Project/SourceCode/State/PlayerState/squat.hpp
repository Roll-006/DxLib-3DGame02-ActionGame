#pragma once
#include "../../Interface/i_action_state.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class Squat final : public IActionState<Player>
	{
	public:
		Squat();
		~Squat() override;

		void Update(Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(const Player* obj) override;

	private:

	};
}
