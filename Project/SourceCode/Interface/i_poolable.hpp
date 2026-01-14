#pragma once

/// @brief プールに格納されるオブジェクト
class IPoolable abstract
{
public:
	virtual ~IPoolable() = default;

	[[nodiscard]] virtual const bool IsReturnPool() abstract;
};
