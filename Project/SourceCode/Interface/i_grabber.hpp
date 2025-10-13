#pragma once

class IGrabber abstract
{
public:
	virtual ~IGrabber() = default;

	virtual void Grab()				abstract;
	virtual void Release()			abstract;
	virtual void OnEscape()			abstract;

	[[nodiscard]] virtual float GetDamageOverTimeStartTime()	const abstract;
	[[nodiscard]] virtual bool	IsTargetEscaped()				const abstract;
};
