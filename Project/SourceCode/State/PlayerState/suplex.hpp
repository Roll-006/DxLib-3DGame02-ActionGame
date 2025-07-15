#pragma once
#include "../../Interface/i_special_state.hpp"
#include "../../Object/player.hpp"

namespace player_state
{
	class Suplex final : public ISpecialState<Player>
	{
	public:
		Suplex();
		~Suplex() override;

		void Update(const Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(const Player* obj) override;

	private:

	};
}