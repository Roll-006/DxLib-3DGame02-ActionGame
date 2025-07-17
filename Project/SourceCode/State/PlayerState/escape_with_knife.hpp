#pragma once
#include "../../Base/special_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class EscapeWithKnife final : public SpecialStateBase<Player>
	{
	public:
		EscapeWithKnife();
		~EscapeWithKnife() override;

		void Update(Player* obj) override;
		void Enter (Player* obj) override;
		void Exit  (Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(const Player* obj) override;

	private:

	};
}
