#pragma once
#include <memory>
#include "../Interface/i_melee_hittable.hpp"

class MeleeTargetIcon final
{
public:
	MeleeTargetIcon(std::shared_ptr<IMeleeHittable>& melee_target);
	~MeleeTargetIcon();

	void LateUpdate();
	void Draw() const;

private:
	std::shared_ptr<IMeleeHittable>& m_melee_target;
};
