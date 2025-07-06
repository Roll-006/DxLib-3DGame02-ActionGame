#include "ui_manager.hpp"

UIManager::UIManager()
{

}

UIManager::~UIManager()
{

}

void UIManager::Remove(const std::string& name)
{
	m_ui_creator.erase(name);
}

void UIManager::Draw()
{
	for (const auto& ui : m_ui_creator)
	{
		if (ui.second->IsActive())
		{
			ui.second->OnDraw();
		}
	}
}
