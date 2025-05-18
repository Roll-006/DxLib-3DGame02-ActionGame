#pragma once
#include <memory>

#include "../Data/Name/obj_name.hpp"
#include "../Data/Tag/obj_tag.hpp"

#include "../Support/transform.hpp"

class ObjManager;

class ObjBase
{
public:
	ObjBase(const std::string& name, const std::string& tag, const VECTOR& pos) : 
		m_name		(name), 
		m_tag		(tag), 
		m_transform	(std::make_shared<Transform>(pos))
	{ }

	virtual ~ObjBase() = default;

	virtual void Init()			= 0;
	virtual void Update()		= 0;
	virtual void Draw()const	= 0;

	[[nodiscard]] std::string GetName()const { return m_name; }
	[[nodiscard]] std::string GetTag() const { return m_tag; }
	[[nodiscard]] std::shared_ptr<Transform> GetTransform() { return m_transform; }

protected:
	std::shared_ptr<Transform> m_transform;

private:
	std::string m_name;	// オブジェクトの名前
	std::string m_tag;	// オブジェクトの分類
};
