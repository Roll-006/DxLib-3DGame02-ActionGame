#pragma once
#include "../../Base/special_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class SpecialNull final : public SpecialStateBase<Player>
	{
	public:
		SpecialNull();
		~SpecialNull() override;

		void Update(Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(const Player* obj) override;

	private:

	};
}
