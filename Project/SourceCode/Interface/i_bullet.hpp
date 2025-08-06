#pragma once
class GunBase;

class IBullet abstract
{
public:
	virtual ~IBullet() = default;

	virtual void OnShot(const GunBase& gun) abstract;

protected:
	virtual void CalcRayPos() abstract;
};
