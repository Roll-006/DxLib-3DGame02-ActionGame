#pragma once

class ITab abstract
{
public:
	virtual ~ITab() = default;

	virtual bool IsDraw() const abstract;
};
