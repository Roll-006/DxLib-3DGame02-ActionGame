#pragma once
#include <memory>
#include <unordered_map>

#include "../Base/singleton_base.hpp"
#include "../Base/obj_base.hpp"

class ObjManager : public SingletonBase
{
public:
	void AddObj	  (const std::shared_ptr<ObjBase> obj);
	void RemoveObj(const std::string& obj_name);

	[[nodiscard]] std::shared_ptr<ObjBase> GetObj(const std::string& obj_name)noexcept;

private:
	ObjManager();
	~ObjManager()override;

private:
	std::unordered_map<std::string, std::shared_ptr<ObjBase>> m_objects;
};
