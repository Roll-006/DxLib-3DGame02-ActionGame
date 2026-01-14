#pragma once
#include <memory>
#include "../Part/transform.hpp"
#include "../Part/modeler.hpp"

class ILoadableAmmo abstract
{
public:
	virtual ~ILoadableAmmo() = default;

	virtual void OnStartReload(const std::shared_ptr<Modeler>& owner_modler) abstract;
	virtual void OnReloaded() abstract;
	virtual void TrackLoad() abstract;

	[[nodiscard]] virtual const bool IsReloading() const abstract;

protected:
	virtual void TrackOwnerHand() abstract;
};
