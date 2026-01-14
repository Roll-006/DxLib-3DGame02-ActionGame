#pragma once
#include <memory>
#include "../Part/humanoid_frame_getter.hpp"

class IHumanoid abstract
{
public:
	virtual ~IHumanoid() = default;

	[[nodiscard]] virtual const std::shared_ptr<const HumanoidFrameGetter> GetHumanoidFrame() const abstract;
};
