#pragma once
#include <string>
#include <unordered_map>

#include "../Base/singleton_base.hpp"
#include "../Base/obj_base.hpp"
#include "../Concept/obj_concepts.hpp"

class ObjManager final : public SingletonBase<ObjManager>
{
public:
	template<obj_concepts::ObjT ObjT>
	void AddObj(const std::shared_ptr<ObjT> obj)
	{
		// 上書き不可
		if (!m_objects.count(obj->GetName()))
		{
			m_objects[obj->GetName()] = obj;
		}
	}

	void RemoveObj(const std::string& obj_name);

	/// @brief オブジェクトが存在しているかを判定
	[[nodiscard]] bool IsExist(const std::string& obj_name) { return m_objects.count(obj_name); }

	[[nodiscard]] std::shared_ptr<ObjBase> GetObj(const std::string& obj_name);

private:
	ObjManager();
	~ObjManager() override;

private:
	std::unordered_map<std::string, std::shared_ptr<ObjBase>> m_objects;

	friend SingletonBase<ObjManager>;
};
