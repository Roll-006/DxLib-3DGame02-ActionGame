#pragma once

class IPlayableCharacter abstract
{
public:
	virtual ~IPlayableCharacter() = default;

	virtual void OnAllowControl()	 abstract;
	virtual void OnDisallowControl() abstract;

	[[nodiscard]] virtual const bool CanControl() const abstract;
};
