#pragma once

/// @brief ƒƒŒ[UŒ‚‚ğó‚¯‚é‘¤
class IMeleeHittable abstract
{
public:
	virtual ~IMeleeHittable() = default;

	virtual [[nodiscard]] const bool IsStandStun()  const abstract;
	virtual [[nodiscard]] const bool IsCrouchStun() const abstract;
};
