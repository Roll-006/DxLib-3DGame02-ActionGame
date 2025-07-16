#pragma once
#include "../../Base/move_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class MoveNull final : public MoveStateBase<Player>
	{
	public:
		MoveNull();
		~MoveNull() override;

		void Update(Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(const Player* obj) override;

	private:
		float m_non_move_time;
	};
}
