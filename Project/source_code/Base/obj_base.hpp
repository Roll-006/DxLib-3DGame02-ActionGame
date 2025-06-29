#pragma once
#include <memory>

#include "../Data/Name/obj_name.hpp"
#include "../Data/Tag/obj_tag.hpp"

#include "../Part/transform.hpp"
#include "../Part/modeler.hpp"

class ObjManager;

class ObjBase abstract
{
public:
	ObjBase(const std::string& name, const std::string& tag) : 
		m_transform	(std::make_shared<Transform>()),
		m_modeler	(nullptr),
		m_name		(name), 
		m_tag		(tag),
		m_is_active (true)
	{ }

	virtual ~ObjBase() = default;

	virtual void Init()			abstract;
	virtual void Update()		abstract;
	virtual void LateUpdate()   abstract;
	virtual void Draw() const	abstract;

	/// @brief オブジェクトをアクティブ化する
	void Activate()   { m_is_active = true; }
	/// @brief オブジェクトを非アクティブ化する(削除せずに機能を停止)
	void Deactivate() { m_is_active = false; }

	[[nodiscard]] std::string GetName() const { return m_name; }
	[[nodiscard]] std::string GetTag()  const { return m_tag; }
	[[nodiscard]] std::shared_ptr<Transform> GetTransform() { return m_transform; }
	[[nodiscard]] std::shared_ptr<Modeler>   GetModeler()   { return m_modeler; }

	[[nodiscard]] bool IsActive() const { return m_is_active; }

protected:
	std::shared_ptr<Transform> m_transform;
	std::shared_ptr<Modeler>   m_modeler;

	std::string m_name;	// オブジェクトの名前
	std::string m_tag;	// オブジェクトの分類

	bool m_is_active;
};
