#pragma once

class IGrabber abstract
{
public:
	virtual ~IGrabber() = default;

	virtual void Grab()				abstract;
	virtual void Release()			abstract;
	virtual void OnEscape()			abstract;

	[[nodiscard]] virtual const float GetDamageOverTimeStartTime()	const abstract;
	[[nodiscard]] virtual const bool  IsTargetEscaped()				const abstract;
};
