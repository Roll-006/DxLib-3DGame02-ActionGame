#pragma once
#include "../Data/Name/obj_name.hpp"
#include "../Data/Tag/obj_tag.hpp"

class ObjBase
{
public:
	ObjBase(const std::string& name, const std::string& tag) : m_name(name), m_tag(tag){}
	virtual ~ObjBase() = default;

	virtual void Init()			= 0;
	virtual void Update()		= 0;
	virtual void Draw()const	= 0;

	[[nodiscard]] std::string GetName()const { return m_name; }
	[[nodiscard]] std::string GetTag() const { return m_tag; }

private:
	std::string m_name;	// オブジェクトの名前
	std::string m_tag;	// オブジェクトの分類
};
