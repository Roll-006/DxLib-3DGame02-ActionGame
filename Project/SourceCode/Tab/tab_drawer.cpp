#include "tab_drawer.hpp"

TabDrawer::TabDrawer()
{

}

TabDrawer::~TabDrawer()
{

}

void TabDrawer::RemoveTab(const std::type_index& type)
{
	if (!m_tabs.count(type)) { return; }

	m_tabs.erase(type);

	// —Dæ‡ˆÊ‚à“¯‚Éíœ
	const auto remove = std::find_if(m_draw_priority.begin(), m_draw_priority.end(), [=](const std::pair<std::type_index, int> p)
	{
		return p.first == type;
	});

	if (remove != m_draw_priority.end())
	{
		m_draw_priority.erase(remove);
	}
}

void TabDrawer::Draw() const
{
	for (const auto& priority : m_draw_priority)
	{
		if (m_tabs.at(priority.first)->IsActive())
		{
			m_tabs.at(priority.first)->OnDraw();
		}
	}
}

void TabDrawer::Activate(const std::type_index& type)
{
	for (const auto& ui : m_tabs)
	{
		if (ui.first == type)
		{
			ui.second->Activate();
		}
	}
}

void TabDrawer::Deactivate(const std::type_index& type)
{
	for (const auto& ui : m_tabs)
	{
		if (ui.first == type)
		{
			ui.second->Deactivate();
		}
	}
}
