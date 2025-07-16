#pragma once
#include "../../Interface/i_special_state.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class RoundhouseKick final : public ISpecialState<Player>
	{
	public:
		RoundhouseKick();
		~RoundhouseKick() override;

		void Update(Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(const Player* obj) override;

	private:

	};
}
