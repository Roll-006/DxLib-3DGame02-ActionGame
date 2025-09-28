#pragma once

class GunBase;

class IBullet abstract
{
public:
	virtual ~IBullet() = default;

	virtual void OnShot(GunBase& gun) abstract;

	[[nodiscard]] virtual float			GetPower()			const abstract;
	[[nodiscard]] virtual std::string	GetShotOwnerName()	const abstract;

protected:
	virtual void CalcRayCastPos() abstract;
};
