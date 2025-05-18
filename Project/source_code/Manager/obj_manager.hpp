#pragma once
#include <string>
#include <unordered_map>

#include "../Base/singleton_base.hpp"
#include "../Base/obj_base.hpp"
#include "../Concept/obj_concepts.hpp"

class ObjManager : public SingletonBase<ObjManager>
{
public:
	friend SingletonBase<ObjManager>;

	template<obj_concepts::DerivedT ObjT>
	void AddObj(const std::shared_ptr<ObjT> obj)
	{
		// ã‘‚«•s‰Â
		if (!m_objects.count(obj->GetName()))
		{
			m_objects[obj->GetName()] = obj;
		}
	}

	void RemoveObj(const std::string& obj_name);

	[[nodiscard]] std::shared_ptr<ObjBase> GetObj(const std::string& obj_name)noexcept;

private:
	ObjManager();
	~ObjManager()override;

private:
	std::unordered_map<std::string, std::shared_ptr<ObjBase>> m_objects;
};
