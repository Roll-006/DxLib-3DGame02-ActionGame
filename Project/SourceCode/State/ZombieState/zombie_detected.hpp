#pragma once
#include "../../Base/zombie_state_base.hpp"

class Animator;

namespace zombie_state
{
	class Detected final : public ZombieStateBase
	{
	public:
		Detected(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator);
		~Detected();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const ZombieStateKind GetNextStateKind() override;
	};
}
