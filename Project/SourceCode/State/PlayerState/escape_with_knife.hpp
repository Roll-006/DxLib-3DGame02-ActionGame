#pragma once
#include "../../Interface/i_state.hpp"
#include "../../Object/player.hpp"

namespace player_state
{
	class EscapeWithKnife final : public IState<Player>
	{
	public:
		EscapeWithKnife();
		~EscapeWithKnife() override;

		void Update(const Player* obj) override;
		void Enter(const Player* obj) override;
		[[nodiscard]] IState<Player>* ChangeState(const Player* obj) override;

	private:

	};
}
