#pragma once
#include "../Name/obj_name.hpp"
#include "../Tag/obj_tag.hpp"

#include "../Part/transform.hpp"

class ObjAccessor;

class ObjBase abstract : public std::enable_shared_from_this<ObjBase>
{
public:
	ObjBase(const std::string& name, const std::string& tag);
	virtual ~ObjBase() = default;

	virtual void Init()						abstract;
	virtual void Update()					abstract;
	virtual void LateUpdate()				abstract;
	virtual void Draw()				const	abstract;

	/// @brief 各マネージャーに自身を登録する
	virtual void AddToObjManager()			abstract;
	virtual void RemoveToObjManager()		abstract;

	/// @brief オブジェクトをアクティブ化する
	void Activate()   { m_is_active = true; }
	/// @brief オブジェクトを非アクティブ化する(削除せずに機能を停止)
	void Deactivate() { m_is_active = false; }


	#pragma region Getter
	[[nodiscard]] const virtual float				GetDeltaTime()	const abstract;
	[[nodiscard]] const std::shared_ptr<Transform>	GetTransform()	const	{ return m_transform; }
	[[nodiscard]] const std::string					GetName()		const	{ return m_name; }
	[[nodiscard]] const std::string					GetTag()		const	{ return m_tag; }
	[[nodiscard]] const int							GetObjHandle()	const	{ return m_obj_handle; }
	[[nodiscard]] const bool						IsActive()		const	{ return m_is_active; }
	#pragma endregion

protected:
	std::shared_ptr<Transform> m_transform;

private:
	std::string m_name;		// オブジェクトの名前
	std::string m_tag;		// オブジェクトの分類

	bool m_is_active;
	int  m_obj_handle;

	friend void from_json	(const nlohmann::json& j_data, ObjBase& obj_base);
	friend void to_json		(nlohmann::json& j_data, const ObjBase& obj_base);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, ObjBase& obj_base)
{
	j_data.at("transform").get_to(*obj_base.m_transform.get());
}

inline void to_json(nlohmann::json& j_data, const ObjBase& obj_base)
{
	j_data = nlohmann::json
	{
		{ "transform", *obj_base.m_transform.get() },
	};
}
#pragma endregion
