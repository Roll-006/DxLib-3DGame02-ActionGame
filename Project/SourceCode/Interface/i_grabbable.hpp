#pragma once
#include <DxLib.h>

class IGrabbable abstract
{
public:
	virtual ~IGrabbable() = default;

	virtual void OnGrabbed(const VECTOR& brabber_pos, const VECTOR& brabber_dir) abstract;
	virtual void OnRelease() abstract;

	virtual void OnGrabbedDamage(const float damage) abstract;

	[[nodiscard]] virtual bool IsGrabbed() const abstract;
};
