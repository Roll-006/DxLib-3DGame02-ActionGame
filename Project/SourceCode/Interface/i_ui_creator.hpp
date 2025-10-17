#pragma once
#include "../Name/ui_creator_name.hpp"

class IUICreator abstract
{
public:
	virtual ~IUICreator() = default;

	virtual void Init() abstract;
	virtual void LateUpdate() abstract;
	virtual void OnDraw(const int main_screen_handle) abstract;

	virtual void Activate() 	abstract;
	virtual void Deactivate()	abstract;

	[[nodiscard]] virtual std::string GetName()		const abstract;
	[[nodiscard]] virtual int		  GetPriority() const abstract;
	[[nodiscard]] virtual bool		  IsActive()	const abstract;
};
