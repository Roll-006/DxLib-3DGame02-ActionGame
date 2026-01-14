#pragma once
class Zombie;
class Animator;
class ZombieStateBase;
enum class ZombieStateKind;

namespace zombie_state
{
	class State final
	{
	public:
		State(Zombie& zombie, const std::shared_ptr<Animator>& animator);
		~State();

		void Update();
		void LateUpdate();

		[[nodiscard]] const ZombieStateKind GetPrevStateKind()	const;
		[[nodiscard]] const ZombieStateKind GetCurrentStateKind() const;

		#pragma region Try判定
		[[nodiscard]] bool TryWaitForcibly();
		[[nodiscard]] bool TryPatrol();
		[[nodiscard]] bool TryTrack();
		[[nodiscard]] bool TryRunAttack();
		[[nodiscard]] bool TryDetected();
		[[nodiscard]] bool TryWalk();
		[[nodiscard]] bool TryRun();
		[[nodiscard]] bool TryStealthKilled();
		[[nodiscard]] bool TryGrabRun();
		[[nodiscard]] bool TryKnockback();
		[[nodiscard]] bool TryBackwardKnockback();
		[[nodiscard]] bool TryDead();
		[[nodiscard]] bool TryLeftCrouchStun();
		[[nodiscard]] bool TryRightCrouchStun();
		[[nodiscard]] bool TryStandStun();
		#pragma endregion

	private:
		Zombie&																	m_zombie;
		std::shared_ptr<ZombieStateBase>										m_current_state;
		ZombieStateKind															m_prev_state_kind;
		std::unordered_map<ZombieStateKind, std::shared_ptr<ZombieStateBase>>	m_states;
	};
}
