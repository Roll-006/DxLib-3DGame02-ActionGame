#pragma once

class GunBase;

class IBullet abstract
{
public:
	virtual ~IBullet() = default;

	virtual void OnShot(GunBase& gun) abstract;

	[[nodiscard]] virtual const float		GetPower()			const abstract;
	[[nodiscard]] virtual const std::string	GetShotOwnerName()	const abstract;

protected:
	virtual void CalcRayCastPos() abstract;
};
