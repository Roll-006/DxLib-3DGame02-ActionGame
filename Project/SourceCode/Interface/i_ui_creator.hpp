#pragma once
#include "../Data/Name/ui_creator_name.hpp"

class IUICreator abstract
{
public:
	virtual ~IUICreator() = default;

	virtual void LateUpdate()	abstract;
	virtual void OnDraw()		abstract;

	/// @brief アクティブ化する
	virtual void Activate() 	abstract;
	/// @brief 非アクティブ化する(削除せずに機能を停止)
	virtual void Deactivate()	abstract;

	virtual [[nodiscard]] std::string GetName()		const abstract;
	virtual [[nodiscard]] int		  GetPriority() const abstract;
	virtual [[nodiscard]] bool		  IsActive()	const abstract;
};
