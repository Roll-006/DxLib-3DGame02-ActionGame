#pragma once

class IShooter abstract
{
public:
	virtual ~IShooter() = default;

	/// @brief c’e”‚ğİ’è‚·‚é
	/// @param remaining_bullet_num 
	void SetRemainingBulletNum(const int remaining_bullet_num);

	[[nodiscard]] int GetCurrentRemainingBulletNum() const;
};
