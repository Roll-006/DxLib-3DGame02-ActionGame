#pragma once
#include "../../Interface/i_special_state.hpp"
#include "../../Object/player.hpp"

namespace player_state
{
	class FinishOff final : public ISpecialState<Player>
	{
	public:
		FinishOff();
		~FinishOff() override;

		void Update(const Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] IState<Player>* ChangeState(const Player* obj) override;

	private:

	};
}
