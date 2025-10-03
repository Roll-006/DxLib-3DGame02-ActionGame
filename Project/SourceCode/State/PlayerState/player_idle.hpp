#pragma once
#include "../../Base/move_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class Idle final : public MoveStateBase<Player>
	{
	public:
		Idle();
		~Idle() override;

		void Update		(std::shared_ptr<Player>& obj) override;
		void LateUpdate	(std::shared_ptr<Player>& obj) override;
		void Enter		(std::shared_ptr<Player>& obj) override;
		void Exit		(std::shared_ptr<Player>& obj) override;

		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(std::shared_ptr<Player>& obj) override;
		[[nodiscard]] bool IsAllStop() const override { return m_is_all_stop; }

	private:
		float m_non_move_time;
		bool  m_is_all_stop;
	};
}
