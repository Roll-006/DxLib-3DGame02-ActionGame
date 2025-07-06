#pragma once
#include "../Data/Name/ui_creator_name.hpp"

class UICreatorBase abstract
{
public:
	UICreatorBase(const std::string& name) : m_name(name), m_is_active(true){}
	virtual ~UICreatorBase() = default;

	virtual void LateUpdate()	abstract;
	virtual void OnDraw()		abstract;

	/// @brief オブジェクトをアクティブ化する
	void Activate()		{ m_is_active = true; }
	/// @brief オブジェクトを非アクティブ化する(削除せずに機能を停止)
	void Deactivate()	{ m_is_active = false; }

	[[nodiscard]] std::string GetName() const { return m_name; }
	[[nodiscard]] bool IsActive() const { return m_is_active; }

protected:
	std::string m_name;
	bool		m_is_active;
};
