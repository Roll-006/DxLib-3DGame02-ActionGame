#pragma once
#include <memory>

class IAmmoBox abstract
{
public:
	virtual ~IAmmoBox() = default;

	/// @brief ’e–ò‚ğ‡¬‚·‚é
	virtual void Synthesize(const std::shared_ptr<IAmmoBox> ammo_box) abstract;

	virtual void SetHaveNum(const int set_num) abstract;
	virtual void AddHaveNum(const int add_num) abstract;

	[[nodiscard]] virtual int	GetMaxHaveNum()		const abstract;
	[[nodiscard]] virtual int	GetCurrentHaveNum()	const abstract;
	[[nodiscard]] virtual bool	IsMax()				const abstract;
};
