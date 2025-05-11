#include "game_obj_manager.hpp"

GameObjManager* GameObjManager::m_instance = nullptr;

void GameObjManager::Generate()
{
	if (!m_instance)
	{
		m_instance = new GameObjManager;
	}
}

void GameObjManager::Delete()
{
	delete m_instance;
	m_instance = nullptr;
}

GameObjManager* GameObjManager::GetInstance()
{
	return m_instance ? m_instance : nullptr;
}

GameObjManager::GameObjManager()
{

}

GameObjManager::~GameObjManager()
{
	// èàóùÇ»Çµ
}

void GameObjManager::AddObj(const std::shared_ptr<GameObj> obj)
{
	// è„èëÇ´ïsâ¬
	if (!m_objects.count(obj->GetName()))
	{
		m_objects[obj->GetName()] = obj;
	}
}

void GameObjManager::RemoveObj(const std::string& obj_name)
{
	m_objects.erase(obj_name);
}

std::shared_ptr<GameObj> GameObjManager::GetObj(const std::string& obj_name)
{
	return m_objects.count(obj_name) ? m_objects.at(obj_name) : nullptr;
}
