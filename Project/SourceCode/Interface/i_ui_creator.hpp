#pragma once
#include "../Name/ui_creator_name.hpp"

class IUICreator abstract
{
public:
	virtual ~IUICreator() = default;

	virtual void LateUpdate() abstract;
	virtual void OnDraw(const int main_screen_handle) abstract;

	/// @brief アクティブ化する
	virtual void Activate() 	abstract;
	/// @brief 非アクティブ化する(削除せずに機能を停止)
	virtual void Deactivate()	abstract;

	[[nodiscard]] virtual std::string GetName()		const abstract;
	[[nodiscard]] virtual int		  GetPriority() const abstract;
	[[nodiscard]] virtual bool		  IsActive()	const abstract;
};
