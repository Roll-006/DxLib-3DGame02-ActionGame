#pragma once
#include <DxLib.h>

class IGrabbable abstract
{
public:
	virtual ~IGrabbable() = default;

	virtual void OnGrabbed(const VECTOR& brabber_dir) abstract;

	[[nodiscard]] virtual bool IsGrabbed() const abstract;
};
