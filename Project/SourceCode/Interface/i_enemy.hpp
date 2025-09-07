#pragma once

class IEnemy abstract
{
public:
	virtual ~IEnemy() = default;

	[[nodiscard]] virtual bool IsTargetInSight(const int target_model_handle) abstract;
	[[nodiscard]] virtual bool CanAttack() const abstract;

protected:

};
