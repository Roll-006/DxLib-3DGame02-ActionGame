#pragma once
class GunBase;

class IBullet abstract
{
public:
	virtual ~IBullet() = default;

	virtual void OnShot(GunBase& gun) abstract;
	virtual [[nodiscard]] std::string GetShotOwnerName() const abstract;

protected:
	virtual void CalcRayPos() abstract;
};
