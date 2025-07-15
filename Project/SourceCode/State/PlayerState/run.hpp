#pragma once
#include "../../Interface/i_action_state.hpp"
#include "../../Object/player.hpp"

namespace player_state
{
	class Run final : public IActionState<Player>
	{
	public:
		Run();
		~Run() override;

		void Update(const Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] IState<Player>* ChangeState(const Player* obj) override;

	private:

	};
}
