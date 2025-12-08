#pragma once
#include <memory>
#include "../Part/humanoid_frame_getter.hpp"

class IHumanoid abstract
{
public:
	virtual ~IHumanoid() = default;

	[[nodiscard]] virtual std::shared_ptr<HumanoidFrameGetter> GetHumanoidFrame() const abstract;
};
