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
		m_tag		(tag)
	{ }

	virtual ~ObjBase() = default;

	virtual void Init()			abstract;
	virtual void Update()		abstract;
	virtual void Draw() const	abstract;

	/// @brief モデルの作成
	/// @brief 上書き不可
	void MakeModel(const std::string& file_path, const VECTOR& basic_angle)
	{
		if (m_modeler != nullptr) { return; }
		m_modeler = std::make_shared<Modeler>(m_transform, file_path, basic_angle);
	}
	void MakeModel(const std::string& file_path)
	{
		if (m_modeler != nullptr) { return; }
		m_modeler = std::make_shared<Modeler>(m_transform, file_path);
	}

	[[nodiscard]] std::string GetName() const { return m_name; }
	[[nodiscard]] std::string GetTag()  const { return m_tag; }
	[[nodiscard]] std::shared_ptr<Transform> GetTransform() { return m_transform; }
	[[nodiscard]] std::shared_ptr<Modeler>   GetModeler()   { return m_modeler; }

protected:
	std::shared_ptr<Transform> m_transform;
	std::shared_ptr<Modeler>   m_modeler;

	std::string m_name;	// オブジェクトの名前
	std::string m_tag;	// オブジェクトの分類
};
