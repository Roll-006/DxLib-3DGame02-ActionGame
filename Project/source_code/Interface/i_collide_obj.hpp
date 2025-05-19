#pragma once

#include "../Base/obj_base.hpp"

/// @brief 衝突判定を行うオブジェクト
class ICollideObj
{
public:
	virtual ~ICollideObj() = default;

	virtual void OnCollide(const ObjBase* check_hit_obj) = 0;
};
