#pragma once
#include <typeindex>

class IEvent abstract
{
public:
	virtual ~IEvent() = default;

	[[nodiscard]] virtual std::type_index GetType() const abstract;
};
