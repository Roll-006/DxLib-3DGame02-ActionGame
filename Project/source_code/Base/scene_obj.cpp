#include "scene_obj.hpp"

SceneObj::SceneObj(const std::string& name, const std::string& tag) :
	ObjBase(name, tag)
{
	// ˆ—‚È‚µ
}

SceneObj::~SceneObj()
{
	// ˆ—‚È‚µ
}

void SceneObj::Init()
{
	for (auto& obj : m_objects)
	{
		obj.second->Init();
	}
}

void SceneObj::Update()
{
	for (auto& obj : m_objects)
	{
		obj.second->Update();
	}
}

void SceneObj::Draw()const
{
	for (auto& obj : m_objects)
	{
		obj.second->Draw();
	}
}

void SceneObj::AttachObj(const std::shared_ptr<GameObj> obj)
{
	// ã‘‚«•s‰Â
	if (!m_objects.count(obj->GetName()))
	{
		m_objects[obj->GetName()] = obj;
	}
	GameObjManager::GetInstance()->AddObj(obj);
}

void SceneObj::DetachObj(const std::string& obj_name)
{
	m_objects.erase(obj_name);
	GameObjManager::GetInstance()->RemoveObj(obj_name);
}

std::shared_ptr<GameObj> SceneObj::GetObj(const std::string& obj_name)
{
	return m_objects.count(obj_name) ? m_objects.at(obj_name) : nullptr;
}
