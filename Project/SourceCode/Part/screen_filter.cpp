#include "screen_filter.hpp"

ScreenFilter::ScreenFilter() : 
	m_screen_creator(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_is_using		(false)
{
	m_filters[Kind::kNearDeath] = [this] { SetNearDeathFilter(); };
}

ScreenFilter::~ScreenFilter()
{

}

void ScreenFilter::OnNotify(const IEvent& event)
{
	// •mŽ€ó‘Ô‚É“Ë“ü‚µ‚½
	if (event.GetType() == std::type_index(typeid(EnterNearDeathData)))
	{
		m_is_using = true;
	}
}

void ScreenFilter::Update()
{

}

void ScreenFilter::UseFilter()
{
	if (!m_is_using) { return; }

	m_screen_creator->UseScreen();
}

void ScreenFilter::UnuseFilter()
{
	if (!m_is_using) { return; }

	m_screen_creator->UnuseScreen();
	

}

void ScreenFilter::Draw() const
{
	if (!m_is_using) { return; }

	m_screen_creator->Draw();
}

void ScreenFilter::SetNearDeathFilter()
{
	GraphFilter(m_screen_creator->GetScreenHandle(), DX_GRAPH_FILTER_MONO, 0, 0);
}
