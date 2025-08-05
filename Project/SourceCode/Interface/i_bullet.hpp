#pragma once
#include "../Base/gun_base.hpp"

class IBullet abstract
{
public:
	virtual ~IBullet() = default;

	virtual void OnShot(const GunBase& gun) abstract;

protected:
	virtual void CalcRayPos() abstract;
};
