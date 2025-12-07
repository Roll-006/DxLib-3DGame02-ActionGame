#pragma once
#include "../Event/event_system.hpp"

class BattleObserver final
{
public:
	BattleObserver();
	~BattleObserver();

	[[nodiscard]] bool IsInBattle() const { return m_is_in_battle; }

private:
	/// @brief í“¬’†‚Ì“G”‚ğ‰ÁZ‚·‚é
	void AddInBattleCount	(const OnTargetDetectedEvent& event);

	/// @brief í“¬’†‚Ì“G”‚ğŒ¸Z‚·‚é
	void RemoveInBattleCount(const DeadEnemyEvent& event);

private:
	int  m_battle_enemies_count;	// í“¬’†‚Ì“G”
	bool m_is_in_battle;
};
