#pragma once

class ITab abstract
{
public:
	virtual ~ITab() = default;

	virtual void Init()			abstract;
	virtual void Update()		abstract;
	virtual void OnDraw() const abstract;

	[[nodiscard]] virtual bool IsDraw() const abstract;
};
