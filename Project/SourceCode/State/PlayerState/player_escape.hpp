#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
	class Escape final : public PlayerStateBase
	{
	public:
		Escape(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator);
		~Escape();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const PlayerStateKind GetNextStateKind() override;
	};
}
