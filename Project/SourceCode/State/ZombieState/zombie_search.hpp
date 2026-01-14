#pragma once
#include "../../Base/zombie_state_base.hpp"

class Animator;

namespace zombie_state
{
	class Search final : public ZombieStateBase
	{
	public:
		Search(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator);
		~Search();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const ZombieStateKind GetNextStateKind() override;

	private:

	};
}
