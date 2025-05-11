#pragma once
#include <memory>
#include <unordered_map>
#include "obj_base.hpp"

class SceneBase
{
public:
	virtual ~SceneBase() = default;

	virtual void Init()			= 0;
	virtual void Update()		= 0;
	virtual void Draw()const	= 0;

	virtual void AttachObj(const std::shared_ptr<ObjBase> obj)	= 0;
	virtual void DetachObj(const std::string& obj_name)			= 0;

	virtual [[nodiscard]] std::shared_ptr<ObjBase> GetObj(const std::string& obj_name)
	{
		if (m_objects.count(obj_name)) { return m_objects.at(obj_name); }
	}

private:
	std::unordered_map<std::string, std::shared_ptr<ObjBase>> m_objects;	// Key : name, Value : obj
};
