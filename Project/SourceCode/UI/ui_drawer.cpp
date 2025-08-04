#include "ui_drawer.hpp"

UIDrawer::UIDrawer()
{

}

UIDrawer::~UIDrawer()
{

}

void UIDrawer::Remove(const std::string& name)
{
	m_ui_creator.erase(name);
}

void UIDrawer::Draw()
{
	for (const auto& ui : m_ui_creator)
	{
		if (ui.second->IsActive())
		{
			ui.second->OnDraw();
		}
	}
}
