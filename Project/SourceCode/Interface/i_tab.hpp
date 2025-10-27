#pragma once
#include "../Base/ui_button_base.hpp"
#include "../Base/ui_slider_base.hpp"

#include "../Handle/handle_creator.hpp"

class ITab abstract
{
public:
	virtual ~ITab() = default;

	virtual void Init()			abstract;
	virtual void Update()		abstract;
	virtual void OnDraw(const int main_screen_handle) const abstract;

	virtual void Activate() 	abstract;
	virtual void Deactivate()	abstract;

	virtual void AllowSelect()		abstract;
	virtual void DisallowSelect()	abstract;

	[[nodiscard]] virtual int   GetTabHandle()  const abstract;
	[[nodiscard]] virtual int	GetPriority()	const abstract;
	[[nodiscard]] virtual bool	IsActive()		const abstract;
	[[nodiscard]] virtual bool  CanSelect()		const abstract;
};
