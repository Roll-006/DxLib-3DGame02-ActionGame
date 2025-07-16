#pragma once
#include "../../Interface/i_action_state.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class ActionNull final : public IActionState<Player>
	{
	public:
		ActionNull();
		~ActionNull() override;

		void Update(Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(const Player* obj) override;

	private:
		[[nodiscard]] bool TryRun();
		[[nodiscard]] bool TryCrouch();
	};
}
