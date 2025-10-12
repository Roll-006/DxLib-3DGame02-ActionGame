#pragma once

class IGrabber abstract
{
public:
	virtual ~IGrabber() = default;

	virtual void Grab()				abstract;
	virtual void Release()			abstract;
	virtual void ReleaseForcibly()	abstract;

	[[nodiscard]] virtual bool IsRelease() const abstract;
};
