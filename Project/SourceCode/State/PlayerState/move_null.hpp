#pragma once
#include "../../Interface/i_move_state.hpp"
#include "../../Object/player.hpp"

namespace player_state
{
	class MoveNull final : public IMoveState<Player>
	{
	public:
		MoveNull();
		~MoveNull() override;

		void Update(const Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] IState<Player>* ChangeState(const Player* obj) override;

	private:
		float m_non_move_time;
	};
}
