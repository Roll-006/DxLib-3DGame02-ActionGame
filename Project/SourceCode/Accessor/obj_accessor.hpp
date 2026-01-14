#pragma once
#include <string>
#include <unordered_map>

#include "../Base/singleton_base.hpp"
#include "../Base/obj_base.hpp"
#include "../Concept/obj_concepts.hpp"

#include "../Handle/handle_creator.hpp"

class ObjAccessor final : public SingletonBase<ObjAccessor>
{
public:
	template<obj_concepts::ObjT ObjT>
	void AddObj(const std::shared_ptr<ObjT>& obj)
	{
		// 上書き不可
		if (!m_objects.contains(obj->GetObjHandle()))
		{
			m_objects[obj->GetObjHandle()] = obj;
		}
	}
	void RemoveObj(const int obj_handle);

	/// @brief オブジェクトが存在しているかを判定
	[[nodiscard]] bool IsExist(const int obj_handle) { return m_objects.contains(obj_handle); }

	/// @brief オブジェクトを取得
	/// @param obj_handle オブジェクトハンドル
	template<obj_concepts::ObjT ObjT>
	[[nodiscard]] std::shared_ptr<ObjT> GetObj(const int obj_handle)
	{
		if (!m_objects.contains(obj_handle)) { return nullptr; }

		auto s_ptr = m_objects.at(obj_handle).lock();
		if (!s_ptr) { return nullptr; }

		const auto obj = std::dynamic_pointer_cast<ObjT>(s_ptr);
		return obj ? obj : nullptr;
	}

	/// @brief オブジェクトを取得
	/// @param obj_name オブジェクト名(同じキャラクターが存在している場合はオブジェクトハンドルでの取得を推奨)
	template<obj_concepts::ObjT ObjT>
	[[nodiscard]] std::shared_ptr<ObjT> GetObj(const std::string& obj_name)
	{
		for (const auto& obj : m_objects)
		{
			auto s_ptr = obj.second.lock();
			if (!s_ptr) continue;

			if (s_ptr->GetName() == obj_name)
			{
				const auto obj = std::dynamic_pointer_cast<ObjT>(s_ptr);
				return obj ? obj : nullptr;
			}
		}

		return nullptr;
	}

	/// @brief オブジェクトハンドルを生成
	[[nodiscard]] int CreateObjHandle() { return HandleCreator::GetInstance()->CreateHandle(); }

private:
	ObjAccessor();
	~ObjAccessor() override;

private:
	std::unordered_map<int, std::weak_ptr<ObjBase>> m_objects;

	friend SingletonBase<ObjAccessor>;
};
