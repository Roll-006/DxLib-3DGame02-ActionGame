#pragma once

class IEnemy abstract
{
public:
	virtual ~IEnemy() = default;

	virtual void SetAttackIntervalTime() abstract;
	virtual void CalcAttackIntervalTime() abstract;

	[[nodiscard]] virtual bool IsTargetInSight() const abstract;
	[[nodiscard]] virtual bool CanAttack()		 const abstract;

protected:

};
