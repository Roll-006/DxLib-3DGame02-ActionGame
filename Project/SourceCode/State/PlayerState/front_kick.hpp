#pragma once
#include "../../Base/special_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class FrontKick final : public SpecialStateBase<Player>
	{
	public:
		FrontKick();
		~FrontKick() override
			;
		void Update		(std::shared_ptr<Player> obj) override;
		void LateUpdate	(std::shared_ptr<Player> obj) override;
		void Enter		(std::shared_ptr<Player> obj) override;
		void Exit		(std::shared_ptr<Player> obj) override;

		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(std::shared_ptr<Player> obj) override;

	private:

	};
}
