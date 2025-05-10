#include "game_obj.hpp"

GameObj::GameObj(const std::string& name, const std::string& tag) :
	ObjBase(name, tag)
{
	// ˆ—‚È‚µ
}

GameObj::~GameObj()
{
	// ˆ—‚È‚µ
}

void GameObj::Init()
{
	for (auto& comp : m_components)
	{
		comp.second->Init();
	}
}

void GameObj::Update()
{
	if (!m_is_active) { return; }

	for (auto& comp : m_components)
	{
		comp.second->Update();
	}
	for (auto& comp : m_components)
	{
		comp.second->LateUpdate();
	}
}

void GameObj::Draw()const
{
	if (!m_is_active) { return; }

	for (auto& comp : m_components)
	{
		comp.second->Draw();
	}
}
