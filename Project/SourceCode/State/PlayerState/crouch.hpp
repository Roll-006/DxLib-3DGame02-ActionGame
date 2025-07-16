#pragma once
#include "../../Base/action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class Crouch final : public ActionStateBase<Player>
	{
	public:
		Crouch();
		~Crouch() override;

		void Update(Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(const Player* obj) override;

	private:

	};
}
