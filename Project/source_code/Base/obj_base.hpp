#pragma once
#include <memory>

#include "../Data/Name/obj_name.hpp"
#include "../Data/Tag/obj_tag.hpp"

#include "../Support/transform.hpp"

class ObjManager;

class ObjBase abstract
{
public:
	ObjBase(const std::string& name, const std::string& tag) : 
		m_name		(name), 
		m_tag		(tag), 
		m_transform	(std::make_shared<Transform>())
	{ }

	virtual ~ObjBase() = default;

	virtual void Init()			abstract;
	virtual void Update()		abstract;
	virtual void Draw()const	abstract;

	[[nodiscard]] std::string GetName()const { return m_name; }
	[[nodiscard]] std::string GetTag() const { return m_tag; }
	[[nodiscard]] std::shared_ptr<Transform> GetTransform() { return m_transform; }

protected:
	std::shared_ptr<Transform> m_transform;
	std::string m_name;	// オブジェクトの名前
	std::string m_tag;	// オブジェクトの分類
};
