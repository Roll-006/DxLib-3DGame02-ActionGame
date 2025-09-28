#pragma once

/// @brief ƒƒŒ[UŒ‚‚ğó‚¯‚é‘¤
class IMeleeHittable abstract
{
public:
	virtual ~IMeleeHittable() = default;

	virtual [[nodiscard]] bool IsStandStun()  const abstract;
	virtual [[nodiscard]] bool IsCrouchStun() const abstract;
};
