#pragma once
#include "../UI/ui_button.hpp"
#include "../UI/ui_slider.hpp"

class ITab abstract
{
public:
	virtual ~ITab() = default;

	virtual void Init()			abstract;
	virtual void Update()		abstract;
	virtual void OnDraw() const abstract;

	[[nodiscard]] virtual bool IsDraw() const abstract;
};
