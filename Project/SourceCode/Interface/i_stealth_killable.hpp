#pragma once

/// @brief ステルスキルを受ける側
class IStealthKillable abstract
{
public:
	virtual ~IStealthKillable() = default;

	virtual void OnStealthKill()		abstract;
	virtual void ExitStealthKilled()	abstract;

	virtual void DisallowStealthKill() abstract;

	[[nodiscard]] virtual const bool IsAllowStealthKill()		const abstract;
	[[nodiscard]] virtual const bool IsStealthKillerInSight()	const abstract;
	[[nodiscard]] virtual const bool IsStealthKilled()			const abstract;
};
