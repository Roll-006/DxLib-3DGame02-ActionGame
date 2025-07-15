#pragma once
#include "../../Interface/i_action_state.hpp"
#include "../../Object/player.hpp"

namespace player_state
{
	class Squat final : public IActionState<Player>
	{
	public:
		Squat();
		~Squat() override;

		void Update(const Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] IState<Player>* ChangeState(const Player* obj) override;

	private:

	};
}
