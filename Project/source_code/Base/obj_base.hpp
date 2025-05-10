#pragma once
#include <string>
#include <typeindex>
#include <memory>
#include <unordered_map>
#include <DxLib.h>

#include "../Data/Tag/obj_tag.hpp"
#include "../FPS/fps.hpp"

class ObjBase
{
public:
	ObjBase(const std::string& name, const std::string& tag) : 
		m_name		(name),
		m_tag		(tag), 
		m_is_active	(true)
	{ }

	virtual ~ObjBase() = default;

	virtual void Init()			= 0;
	virtual void Update()		= 0;
	virtual void Draw()const	= 0;

	/// @brief アクティブ化する
	void Activate()	  { m_is_active = true; }
	/// @brief 非アクティブ化する
	/// @brief オブジェクトを削除せずに処理を停止
	void Deactivate() { m_is_active = false; }

	[[nodiscard]] bool		  IsActive()const { return m_is_active; }
	[[nodiscard]] std::string GetName()	const { return m_name; }
	[[nodiscard]] std::string GetTag()	const { return m_tag; }

protected:
	std::string m_name;		// オブジェクト名
	std::string m_tag;		// オブジェクト分類
	bool m_is_active;		// アクティブかどうか
};
