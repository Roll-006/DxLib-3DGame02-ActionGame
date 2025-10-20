#pragma once
#include "../Part/screen_creator.hpp"

class IUIElement abstract
{
public:
	virtual ~IUIElement() = default;

	virtual void Init()			abstract;
	virtual void Update()		abstract;
	virtual void Draw() const	abstract;

	virtual void Activate() 	abstract;
	virtual void Deactivate()	abstract;

	[[nodiscard]] virtual bool IsActive() const abstract;
};
