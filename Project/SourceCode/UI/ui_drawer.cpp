#include "ui_drawer.hpp"

UIDrawer::UIDrawer()
{

}

UIDrawer::~UIDrawer()
{

}

void UIDrawer::RemoveUICreator(const std::string& name)
{
	if (!m_ui_creators.count(name)) { return; }

	m_ui_creators.erase(name);

	// —Dæ‡ˆÊ‚à“¯‚Éíœ
	const auto remove = std::find_if(m_draw_priority.begin(), m_draw_priority.end(), [=](const std::pair<std::string, int> p)
	{
		return p.first == name;
	});

	if (remove != m_draw_priority.end())
	{
		m_draw_priority.erase(remove);
	}
}

void UIDrawer::Draw()
{
	for (const auto& priority : m_draw_priority)
	{
		if (m_ui_creators.at(priority.first)->IsActive())
		{
			m_ui_creators.at(priority.first)->OnDraw();
		}
	}
}

void UIDrawer::Activate(const std::string& name)
{
	for (const auto& ui : m_ui_creators)
	{
		if (ui.first == name)
		{
			ui.second->Activate();
		}
	}
}

void UIDrawer::Deactivate(const std::string& name)
{
	for (const auto& ui : m_ui_creators)
	{
		if (ui.first == name)
		{
			ui.second->Deactivate();
		}
	}
}
