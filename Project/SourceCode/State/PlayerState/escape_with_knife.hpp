#pragma once
#include "../../Interface/i_special_state.hpp"
#include "../../Object/player.hpp"

namespace player_state
{
	class EscapeWithKnife final : public ISpecialState<Player>
	{
	public:
		EscapeWithKnife();
		~EscapeWithKnife() override;

		void Update(const Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(const Player* obj) override;

	private:

	};
}
