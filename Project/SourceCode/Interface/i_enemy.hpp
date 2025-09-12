#pragma once

class IEnemy abstract
{
public:
	virtual ~IEnemy() = default;

	virtual void SetAttackIntervalTime() abstract;
	virtual void CalcAttackIntervalTime() abstract;

	[[nodiscard]] virtual bool IsTargetInSight(const int target_model_handle) abstract;
	[[nodiscard]] virtual bool CanAttack() const abstract;

protected:

};
