#pragma once
class Player;
class Animator;
class PlayerStateBase;
enum class PlayerStateKind;

namespace player_state
{
	class State final
	{
	public:
		State(Player& player, const std::shared_ptr<Animator>& animator);
		~State();

		void Update();
		void LateUpdate();

		[[nodiscard]] const PlayerStateKind GetPrevStateKind()	const;
		[[nodiscard]] const PlayerStateKind GetCurrentStateKind() const;

		#pragma region Try判定
		[[nodiscard]] const bool TryDead();
		[[nodiscard]] const bool TryGrabbed();
		/// @brief WARNING : TryRoundhouseKickより前に呼び出す必要あり
		[[nodiscard]] const bool TryFrontKick();
		/// @brief WARNING : TryFrontKickより後に呼び出す必要あり
		[[nodiscard]] const bool TryRoundhouseKick();
		[[nodiscard]] const bool TryStealthKill();
		[[nodiscard]] const bool TryEquipKnifeShortcut();
		[[nodiscard]] const bool TryAimKnife();
		[[nodiscard]] const bool TryFirstSideSlashKnife();
		[[nodiscard]] const bool TrySpinningSlash();
		[[nodiscard]] const bool TryEquipGun();
		[[nodiscard]] const bool TryEquipGunShortcut();
		[[nodiscard]] const bool TryPullTrigger();
		[[nodiscard]] const bool TryReload();
		[[nodiscard]] const bool TryPullTriggerReload();
		#pragma endregion

	private:
		Player&																	m_player;
		std::shared_ptr<PlayerStateBase>										m_current_state;
		PlayerStateKind															m_prev_state_kind;
		std::unordered_map<PlayerStateKind, std::shared_ptr<PlayerStateBase>>	m_states;
	};
}
