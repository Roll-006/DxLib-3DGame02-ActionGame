#pragma once

/// @brief ステルスキルを受ける側
class IStealthKillable abstract
{
public:
	virtual ~IStealthKillable() = default;

	virtual void OnStealthKill()		abstract;
	virtual void ExitStealthKilled()	abstract;

	virtual void DisallowStealthKill() abstract;

	[[nodiscard]] virtual bool IsAllowStealthKill()		const abstract;
	[[nodiscard]] virtual bool IsStealthKillerInSight() const abstract;
	[[nodiscard]] virtual bool IsStealthKilled()		const abstract;
};
