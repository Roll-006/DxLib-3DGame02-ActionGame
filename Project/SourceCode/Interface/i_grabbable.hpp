#pragma once
#include <DxLib.h>
#include <memory>

#include "i_grabber.hpp"
#include "../Part/gauge.hpp"

class IGrabbable abstract
{
public:
	virtual ~IGrabbable() = default;

	virtual void UpdateGrabbed() abstract;

	virtual void OnGrabbed(const std::shared_ptr<IGrabber> grabber, const VECTOR& brabber_pos, const VECTOR& brabber_dir) abstract;
	virtual void OnRelease() abstract;

	[[nodiscard]] virtual const std::shared_ptr<const IGrabber>	GetGrabber()		const abstract;
	[[nodiscard]] virtual const std::shared_ptr<const Gauge>	GetEscapeGauge()	const abstract;
	[[nodiscard]] virtual const bool						CanEscape()			const abstract;
	[[nodiscard]] virtual const bool						IsEscape()			const abstract;
	[[nodiscard]] virtual const bool						IsGrabbed()			const abstract;
};
