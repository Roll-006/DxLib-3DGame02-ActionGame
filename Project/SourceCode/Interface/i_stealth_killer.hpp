#pragma once
#include <memory>
#include "i_stealth_killable.hpp"

/// @brief ステルスキルを行う側
class IStealthKiller abstract
{
public:
	virtual ~IStealthKiller() = default;

	virtual void UpdateStealthKill() abstract;

	virtual void StopSearchStealthKillTarget() abstract;

	virtual void SetupStealthKill() abstract;

	virtual void AddStealthKillTarget(const std::shared_ptr<IStealthKillable>& stealth_kill_target) abstract;
	virtual void RemoveStealthKillTarget() abstract;

	[[nodiscard]] virtual std::shared_ptr<IStealthKillable>& GetStealthKillTarget() abstract;
	[[nodiscard]] virtual const bool CanSearchStealthKillTarget() const abstract;
};
