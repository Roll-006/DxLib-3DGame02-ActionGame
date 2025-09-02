#pragma once

class IEnemy abstract
{
public:
	virtual ~IEnemy() = default;

	[[nodiscard]] virtual bool IsTargetInSight(const VECTOR& target_pos) abstract;

protected:

};
