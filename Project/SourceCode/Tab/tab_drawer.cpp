#include "tab_drawer.hpp"

TabDrawer::TabDrawer()
{

}

TabDrawer::~TabDrawer()
{

}

void TabDrawer::RemoveTab(const int tab_handle)
{
	if (!m_tabs.contains(tab_handle)) { return; }

	m_tabs.erase(tab_handle);

	// —Dæ‡ˆÊ‚à“¯‚Éíœ
	const auto remove = std::find_if(m_draw_priority.begin(), m_draw_priority.end(), [=](const std::pair<int, int> p)
	{
		return p.first == tab_handle;
	});

	if (remove != m_draw_priority.end())
	{
		m_draw_priority.erase(remove);
	}
}

void TabDrawer::Draw(const int main_screen_handle) const
{
	for (const auto& priority : m_draw_priority)
	{
		if (m_tabs.at(priority.first)->IsActive())
		{
			m_tabs.at(priority.first)->OnDraw(main_screen_handle);
		}
	}
}

void TabDrawer::Activate(const int tab_handle)
{
	for (const auto& ui : m_tabs)
	{
		if (ui.first == tab_handle)
		{
			ui.second->Activate();
		}
	}
}

void TabDrawer::Deactivate(const int tab_handle)
{
	for (const auto& ui : m_tabs)
	{
		if (ui.first == tab_handle)
		{
			ui.second->Deactivate();
		}
	}
}
